#include "drivers/comlink.h"
#include "drivers/leds.h"
#include "state.h"
#include "log.h"

static bool _is_init = false;


ComLink::ComLink()
: m_log_params_sent(0),
  m_connected(false),
  m_state(NORMAL)
{
    if (_is_init)
        return;
    LOG_ADD_PARAM(U32, "last_packet", &m_last_packet);
    _is_init = true;
}

void ComLink::update()
{
    if (m_connected && ((millis() - m_last_packet) > DISCONNECT_TIMEOUT_MS) ) {
        // If we're connected and timeout occurs, we break the connection.
        m_connected = false;
    }

    m_packet_tx.payload[0] = m_connected;
    m_packet_tx.payload[1] = m_last_packet;

    switch (m_state) {
        case NORMAL:
            receive();
            break;
        case DOWNLOAD_LOG_PARAM:
            sendAvailableLogParam();
        break;
    }

    if (m_connected) {
        led_control.setRx(1);
    } else {
        led_control.setRx(0);
    }
}

bool ComLink::receive()
{
    uint8_t buf[PACKET_SIZE];
    if (!read(buf, PACKET_SIZE)) {
        return false;
    }
    m_last_packet = millis();
    decode_packet(buf, m_packet_rx);
    return respond();
}

bool ComLink::transmit(const packet_type_e type)
{
    m_packet_tx.protocol = PROTOCOL_IN_USE;
    m_packet_tx.version  = PROTOCOL_VERSION;
    m_packet_tx.type  = type;

    uint8_t buf[PACKET_SIZE];
    encode_packet(buf, m_packet_tx);
    write(buf, PACKET_SIZE);
    return true;
}

/* Main method that decides what response should be given. */
bool ComLink::respond()
{
    switch (m_packet_rx.type) {
        case PACKET_TYPE_CONNECT_REQUEST:
            if (!m_connected) {
                transmit(PACKET_TYPE_CONNECT_REQUEST_ACK);
                m_connected = true;
            }
            break;
        case PACKET_TYPE_DISCONNECT:
            if (m_connected) {
                transmit(PACKET_TYPE_DISCONNECT_ACK);
                m_connected = false;
            }
            break;
        case PACKET_TYPE_RAW_MOTOR_THRUST:
            setRawMotorSetpoints();
            transmit(PACKET_TYPE_RAW_MOTOR_THRUST_ACK);
            break;
        case PACKET_TYPE_SETPOINT_ATTITUDE:
            break;
        case PACKET_TYPE_LOGGING_DOWNLOAD:
            m_state = DOWNLOAD_LOG_PARAM;
            break;
        case PACKET_TYPE_STATUS_UPDATE:
            sendStatusUpdate();
            break;
        case PACKET_TYPE_LOGGING_PARAM_READ:
            sendLogParameters();
            break;
        case PACKET_TYPE_ARM_SET:
            state_target.is_armed = m_packet_rx.payload[0];
            transmit(PACKET_TYPE_ARM_SET_ACK);
            break;
        case PACKET_TYPE_PING:
            transmit(PACKET_TYPE_PONG);
            break;
        default:
            transmit(PACKET_TYPE_INCORRECT_PACKET_TYPE);
            m_packet_tx.payload[0] = m_packet_rx.type;
            break;
    }
    return true;
}

/* -- Responses -- */
void ComLink::sendLogParameters()
{
    const size_t nbr_of_params       = (size_t) m_packet_rx.payload[0];
    const log_param_id_t* param_ids  = (log_param_id_t*) &m_packet_rx.payload[1];
    fillPacketLogParams(param_ids, nbr_of_params);
    transmit(PACKET_TYPE_LOGGING_PARAM_READ_ACK);
}

void ComLink::sendAvailableLogParam()
{
    if (m_log_params_sent < Log::availableLogParams()) {
        // Send an available logging parameter.
        fillPacketAvailableLogParam(Log::log_params[m_log_params_sent]);
        transmit(PACKET_TYPE_LOGGING_DOWNLOAD_ACK);
        m_log_params_sent++;
    } else {
        // Indicate that we've sent all logging parameters.
        transmit(PACKET_TYPE_LOGGING_DOWNLOAD_ACK_DONE);
        m_state = NORMAL;
    }
}

void ComLink::sendStatusUpdate()
{
    m_packet_tx.payload[0] = state_estimated.is_armed;
    transmit(PACKET_TYPE_STATUS_UPDATE_ACK);
}

/* -- Helpers -- */
void ComLink::setRawMotorSetpoints()
{
    // Thrust levels for each motor, each thrust is 2 bytes long.
    memcpy(&state_target.motor_raw.m1, &m_packet_rx.payload[0], 2);
    memcpy(&state_target.motor_raw.m2, &m_packet_rx.payload[2], 2);
    memcpy(&state_target.motor_raw.m3, &m_packet_rx.payload[4], 2);
    memcpy(&state_target.motor_raw.m4, &m_packet_rx.payload[6], 2);
}

void ComLink::fillPacketAvailableLogParam(const log_param_t& log_param)
{
    // Fill packet with log parameter data.
    m_packet_tx.payload[0] = log_param.id;
    m_packet_tx.payload[1] = log_param.type;
    m_packet_tx.payload[2] = log_param.name_length;
    memcpy(&m_packet_tx.payload[3], log_param.name, log_param.name_length);
}

size_t ComLink::fillPacketLogParam(const log_param_t& log_param, const uint8_t pkt_pos)
{
    // TODO: Better handling of too big param sizes?
    size_t param_size = Log::get_size(log_param.type);
    memcpy(&m_packet_tx.payload[pkt_pos], log_param.ptr, param_size);
    return param_size;
}

void ComLink::fillPacketLogParams(const log_param_id_t param_ids[], const size_t nbr_of_params)
{
    size_t params_filled = 0;
    size_t pkt_pos       = 0;
    while (pkt_pos < PACKET_PAYLOAD_SIZE && params_filled <= nbr_of_params) {
        log_param_id_t id = param_ids[params_filled];
        pkt_pos += fillPacketLogParam(Log::log_params[id], pkt_pos);
        params_filled++;
    }
}