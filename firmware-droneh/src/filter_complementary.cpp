#include "filter_complementary.h"
#include "math.h"
#include "log.h"


FilterComplementary::FilterComplementary()
: m_alpha(0.2),
  m_acc_est({.x = 0, .y = 0, .z = 0}),
  m_gyro_est({.x = 0, .y = 0, .z = 0})
#ifdef USE_LOWPASS_FILTER_FOR_ACCELERATION
 ,m_moving_average(20)
#endif
{
  LOG_ADD_PARAM(F32, "acc_uf_roll",   &m_acc_est_unfiltered.roll);
  LOG_ADD_PARAM(F32, "acc_uf_pitch",  &m_acc_est_unfiltered.pitch);
  LOG_ADD_PARAM(F32, "acc_f_roll",    &m_acc_est.roll);
  LOG_ADD_PARAM(F32, "acc_f_pitch",   &m_acc_est.pitch);
  LOG_ADD_PARAM(F32, "m_dgyro.roll" , &m_dgyro.roll);
  LOG_ADD_PARAM(F32, "m_dgyro.pitch", &m_dgyro.pitch);
  LOG_ADD_PARAM(F32, "m_dgyro.yaw",   &m_dgyro.yaw);
  LOG_ADD_PARAM(F32, "g_est_roll",    &m_gyro_est.roll);
  LOG_ADD_PARAM(F32, "g_est_pitch",   &m_gyro_est.pitch);
  LOG_ADD_PARAM(F32, "g_est_yaw",     &m_gyro_est.yaw);
}

void FilterComplementary::init()
{}

/* Algorithm inspired by
   1. https://philsal.co.uk/projects/imu-attitude-estimation
   2. http://control.asu.edu/Classes/MMAE441/Aircraft/441Lecture9.pdf
   3. https://www.youtube.com/watch?v=RZd6XDx5VXo
*/
void FilterComplementary::filter(const imu_measurement_t raw_measurement,
                                 const state_t* current_state,
                                 state_t* estimate,
                                 const float dt)
{
    // Estimate roll and pitch from linear acceleration.
    m_acc_est_unfiltered.roll  = atan2(raw_measurement.acc_y, raw_measurement.acc_z);
    m_acc_est_unfiltered.pitch = asin(raw_measurement.acc_x / GRAVITY);

    // Make variable names easier to read.
    float p     = raw_measurement.gyro_x;
    float q     = raw_measurement.gyro_y;
    float r     = raw_measurement.gyro_z;
    float yaw   = current_state->attitude.yaw;
    float roll  = current_state->attitude.roll;
    float pitch = current_state->attitude.pitch;

    // Ensure no division by zero.
    float c_pitch = cos(pitch);
    if (pitch == 0)
      c_pitch = 0.01;

    // Convert gyro data to world coordinates.
    m_dgyro.roll  = p + q*sin(roll)*tan(pitch) + r*cos(roll)*tan(pitch);
    m_dgyro.pitch =     q*cos(roll)            - r*sin(roll);
    m_dgyro.yaw   =     q*sin(roll)/c_pitch    + r*cos(roll)/c_pitch;

    float d_gyro_yaw = .1;
    m_dgyro.yaw = abs(m_dgyro.yaw) < d_gyro_yaw ? 0 : m_dgyro.yaw;

    #ifdef USE_LOWPASS_FILTER_FOR_ACCELERATION
        m_acc_est.roll  = m_moving_average.update(m_acc_est_unfiltered.roll);
        m_acc_est.pitch = m_moving_average.update(m_acc_est_unfiltered.pitch);
    #else
        m_acc_est.roll  = m_acc_est_unfiltered.roll;
        m_acc_est.pitch = m_acc_est_unfiltered.pitch;
    #endif

    // Integrate gyro
    m_gyro_est.roll  = roll  + dt*m_dgyro.roll;
    m_gyro_est.pitch = pitch + dt*m_dgyro.pitch;
    m_gyro_est.yaw   = yaw   + dt*m_dgyro.yaw;

    //Serial.printf("dYaw: %f\n", m_dgyro.yaw);

    //Serial.printf("droll: %f dpitch: %f dyaw: %f \n", d_roll, d_pitch, d_yaw);
    //Serial.printf("roll: %f pitch: %f yaw: %f \n", gyro_est.roll, gyro_est.pitch, gyro_est.yaw);
    //delay(50);

    estimate->attitude.roll  = (1 - m_alpha)*m_gyro_est.roll  + m_alpha*m_acc_est.roll;
    estimate->attitude.pitch = (1 - m_alpha)*m_gyro_est.pitch + m_alpha*m_acc_est.pitch;
    estimate->attitude.yaw   = m_gyro_est.yaw;
}