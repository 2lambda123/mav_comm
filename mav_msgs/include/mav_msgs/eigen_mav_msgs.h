/*
 * Copyright 2015 Fadri Furrer, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Michael Burri, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Markus Achtelik, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Helen Oleynikova, ASL, ETH Zurich, Switzerland
 * Copyright 2015 Mina Kamel, ASL, ETH Zurich, Switzerland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MAV_MSGS_EIGEN_MAV_MSGS_H
#define MAV_MSGS_EIGEN_MAV_MSGS_H

// TODO(helenol): replaced with aligned allocator headers from Simon.
#define MAV_MSGS_CONCATENATE(x, y) x##y
#define MAV_MSGS_CONCATENATE2(x, y) MAV_MSGS_CONCATENATE(x, y)
#define MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EIGEN_TYPE)                    \
  typedef std::vector<EIGEN_TYPE, Eigen::aligned_allocator<EIGEN_TYPE>> \
      MAV_MSGS_CONCATENATE2(EIGEN_TYPE, Vector);                        \
  typedef std::deque<EIGEN_TYPE, Eigen::aligned_allocator<EIGEN_TYPE>>  \
      MAV_MSGS_CONCATENATE2(EIGEN_TYPE, Deque);

#include <Eigen/Eigen>
#include <deque>
#include <iostream>
#include <iostream>

#include "mav_msgs/common.h"

#include <trajectory_msgs/MultiDOFJointTrajectoryPoint.h>
#include "mav_msgs/JointState.h"
#include "mav_msgs/MultiJointTrajectoryPoint.h"

namespace mav_msgs {

/// Actuated degrees of freedom.
enum MavActuation { DOF4 = 4, DOF6 = 6 };

struct EigenAttitudeThrust {
  EigenAttitudeThrust()
      : attitude(Eigen::Quaterniond::Identity()),
        thrust(Eigen::Vector3d::Zero()) {}
  EigenAttitudeThrust(const Eigen::Quaterniond& _attitude,
                      const Eigen::Vector3d& _thrust) {
    attitude = _attitude;
    thrust = _thrust;
  }

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Eigen::Quaterniond attitude;
  Eigen::Vector3d thrust;
};
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenAttitudeThrust)

struct EigenActuators {
  // TODO(ffurrer): Find a proper way of initializing :)

  EigenActuators(const Eigen::VectorXd& _angular_velocities) {
    angular_velocities = _angular_velocities;
  }

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Eigen::VectorXd angles;              // In rad.
  Eigen::VectorXd angular_velocities;  // In rad/s.
  Eigen::VectorXd normalized;          // Everything else, normalized [-1 to 1].
};
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenActuators)

struct EigenRateThrust {
  EigenRateThrust()
      : angular_rates(Eigen::Vector3d::Zero()),
        thrust(Eigen::Vector3d::Zero()) {}

  EigenRateThrust(const Eigen::Vector3d& _angular_rates,
                  const Eigen::Vector3d _thrust)
      : angular_rates(_angular_rates), thrust(_thrust) {}

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Eigen::Vector3d angular_rates;
  Eigen::Vector3d thrust;
};
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenRateThrust)

struct EigenTorqueThrust {
  EigenTorqueThrust()
      : torque(Eigen::Vector3d::Zero()), thrust(Eigen::Vector3d::Zero()) {}

  EigenTorqueThrust(const Eigen::Vector3d& _torque,
                    const Eigen::Vector3d _thrust)
      : torque(_torque), thrust(_thrust) {}

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  Eigen::Vector3d torque;
  Eigen::Vector3d thrust;
};

struct EigenRollPitchYawrateThrust {
  EigenRollPitchYawrateThrust()
      : roll(0.0), pitch(0.0), yaw_rate(0.0), thrust(Eigen::Vector3d::Zero()) {}

  EigenRollPitchYawrateThrust(double _roll, double _pitch, double _yaw_rate,
                              const Eigen::Vector3d& _thrust)
      : roll(_roll), pitch(_pitch), yaw_rate(_yaw_rate), thrust(_thrust) {}

  double roll;
  double pitch;
  double yaw_rate;
  Eigen::Vector3d thrust;
};
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenTorqueThrust)

/**
 * \brief Container holding the state of a MAV: position, velocity, attitude and
 * angular velocity.
 *        In addition, holds the acceleration expressed in body coordinates,
 * which is what the accelerometer
 *        usually measures.
 */
class EigenMavState {
 public:
  typedef std::vector<EigenMavState, Eigen::aligned_allocator<EigenMavState>>
      Vector;
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  /// Initializes all members to zero / identity.
  EigenMavState()
      : position_W(Eigen::Vector3d::Zero()),
        velocity_W(Eigen::Vector3d::Zero()),
        acceleration_B(Eigen::Vector3d::Zero()),
        orientation_W_B(Eigen::Quaterniond::Identity()),
        angular_velocity_B(Eigen::Vector3d::Zero()),
        angular_acceleration_B(Eigen::Vector3d::Zero()) {}

  EigenMavState(const Eigen::Vector3d& position_W,
                const Eigen::Vector3d& velocity_W,
                const Eigen::Vector3d& acceleration_B,
                const Eigen::Quaterniond& orientation_W_B,
                const Eigen::Vector3d& angular_velocity_B,
                const Eigen::Vector3d& angular_acceleration_B)
      : position_W(position_W),
        velocity_W(velocity_W),
        acceleration_B(acceleration_B),
        orientation_W_B(orientation_W_B),
        angular_velocity_B(angular_velocity_B),
        angular_acceleration_B(angular_acceleration_B) {}

  std::string toString() const {
    std::stringstream ss;
    ss << "position:              " << position_W.transpose() << std::endl
       << "velocity:              " << velocity_W.transpose() << std::endl
       << "acceleration_body:     " << acceleration_B.transpose() << std::endl
       << "orientation (w-x-y-z): " << orientation_W_B.w() << " "
       << orientation_W_B.x() << " " << orientation_W_B.y() << " "
       << orientation_W_B.z() << " " << std::endl
       << "angular_velocity_body: " << angular_velocity_B.transpose()
       << std::endl
       << "angular_acceleration_body: " << angular_acceleration_B.transpose()
       << std::endl;

    return ss.str();
  }

  Eigen::Vector3d position_W;
  Eigen::Vector3d velocity_W;
  Eigen::Vector3d acceleration_B;
  Eigen::Quaterniond orientation_W_B;
  Eigen::Vector3d angular_velocity_B;
  Eigen::Vector3d angular_acceleration_B;
};

// reference for a specific joint at a specific time
struct EigenTrajectoryPoint {
  typedef std::vector<EigenTrajectoryPoint,
                      Eigen::aligned_allocator<EigenTrajectoryPoint>>
      Vector;

  //**************//
  // constructors //
  //**************//
  // default constructor
  EigenTrajectoryPoint()
      : timestamp_ns(-1),
        time_from_start_ns(0),
        frame_id("base"),
        parent_id("world"),
        position_W(Eigen::Vector3d::Zero()),
        velocity_W(Eigen::Vector3d::Zero()),
        acceleration_W(Eigen::Vector3d::Zero()),
        jerk_W(Eigen::Vector3d::Zero()),
        snap_W(Eigen::Vector3d::Zero()),
        orientation_W_B(Eigen::Quaterniond::Identity()),
        angular_velocity_W(Eigen::Vector3d::Zero()),
        angular_acceleration_W(Eigen::Vector3d::Zero()),
        angular_jerk_W(Eigen::Vector3d::Zero()),
        angular_snap_W(Eigen::Vector3d::Zero()),
        force_W(Eigen::Vector3d::Zero()),
        torque_W(Eigen::Vector3d::Zero()),
        force_d_W(Eigen::Vector3d::Zero()),
        torque_d_W(Eigen::Vector3d::Zero()),
        degrees_of_freedom(MavActuation::DOF4) {}

  EigenTrajectoryPoint(
      int64_t _time_from_start_ns, const Eigen::Vector3d& _position,
      const Eigen::Vector3d& _velocity, const Eigen::Vector3d& _acceleration,
      const Eigen::Vector3d& _jerk, const Eigen::Vector3d& _snap,
      const Eigen::Quaterniond& _orientation,
      const Eigen::Vector3d& _angular_velocity,
      const Eigen::Vector3d& _angular_acceleration,
      const Eigen::Vector3d& _force, const Eigen::Vector3d& _torque,
      const MavActuation& _degrees_of_freedom = MavActuation::DOF4)
      : time_from_start_ns(_time_from_start_ns),
        position_W(_position),
        velocity_W(_velocity),
        acceleration_W(_acceleration),
        jerk_W(_jerk),
        snap_W(_snap),
        orientation_W_B(_orientation),
        angular_velocity_W(_angular_velocity),
        angular_acceleration_W(_angular_acceleration),
        force_W(_force),
        torque_W(_torque),
        degrees_of_freedom(_degrees_of_freedom) {}

  EigenTrajectoryPoint(
      int64_t _time_from_start_ns, const Eigen::Vector3d& _position,
      const Eigen::Vector3d& _velocity, const Eigen::Vector3d& _acceleration,
      const Eigen::Vector3d& _jerk, const Eigen::Vector3d& _snap,
      const Eigen::Quaterniond& _orientation,
      const Eigen::Vector3d& _angular_velocity,
      const Eigen::Vector3d& _angular_acceleration,
      const MavActuation& _degrees_of_freedom = MavActuation::DOF4)
      : EigenTrajectoryPoint(_time_from_start_ns, _position, _velocity,
                             _acceleration, _jerk, _snap, _orientation,
                             _angular_velocity, _angular_acceleration,
                             Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero(),
                             _degrees_of_freedom) {}

  EigenTrajectoryPoint(
      int64_t _time_from_start_ns, const Eigen::Vector3d& _position,
      const Eigen::Vector3d& _velocity, const Eigen::Vector3d& _acceleration,
      const Eigen::Vector3d& _jerk, const Eigen::Vector3d& _snap,
      const Eigen::Quaterniond& _orientation,
      const Eigen::Vector3d& _angular_velocity,
      const MavActuation& _degrees_of_freedom = MavActuation::DOF4)
      : EigenTrajectoryPoint(_time_from_start_ns, _position, _velocity,
                             _acceleration, _jerk, _snap, _orientation,
                             _angular_velocity, Eigen::Vector3d::Zero(),
                             Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero(),
                             _degrees_of_freedom) {}

  // automatic conversion from JointState message
  EigenTrajectoryPoint(const JointState& msg,
                       const int64_t& time_from_start_ns_ = 0,
                       const int64_t& timestamp_ns_ = 0)
      : timestamp_ns(timestamp_ns_),
        time_from_start_ns(time_from_start_ns_),
        frame_id(msg.frame_id),
        parent_id(msg.parent_id) {
    position_W = vector3FromMsg(msg.transform.translation);
    velocity_W = vector3FromMsg(msg.twist.linear);
    acceleration_W = vector3FromMsg(msg.acceleration.linear);
    jerk_W = vector3FromMsg(msg.jerk.linear);
    snap_W = Eigen::Vector3d::Zero();

    orientation_W_B = quaternionFromMsg(msg.transform.rotation);
    angular_velocity_W = vector3FromMsg(msg.twist.angular);
    angular_acceleration_W = vector3FromMsg(msg.acceleration.angular);
    angular_jerk_W = vector3FromMsg(msg.jerk.angular);
    angular_snap_W = Eigen::Vector3d::Zero();

    force_W = vector3FromMsg(msg.wrench.force);
    torque_W = vector3FromMsg(msg.wrench.torque);
    force_d_W = vector3FromMsg(msg.wrench_derivative.force);
    torque_d_W = vector3FromMsg(msg.wrench_derivative.torque);
  }

  //******************//
  // member variables //
  //******************//
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  // timing
  int64_t timestamp_ns;
  int64_t time_from_start_ns;

  // frames
  std::string frame_id;
  std::string parent_id;

  // translation
  Eigen::Vector3d position_W;
  Eigen::Vector3d velocity_W;
  Eigen::Vector3d acceleration_W;
  Eigen::Vector3d jerk_W;
  Eigen::Vector3d snap_W;

  // rotation
  Eigen::Quaterniond orientation_W_B;
  Eigen::Vector3d angular_velocity_W;
  Eigen::Vector3d angular_acceleration_W;
  Eigen::Vector3d angular_jerk_W;
  Eigen::Vector3d angular_snap_W;

  // interaction wrench
  Eigen::Vector3d force_W;
  Eigen::Vector3d torque_W;
  Eigen::Vector3d force_d_W;
  Eigen::Vector3d torque_d_W;

  // DoF
  MavActuation degrees_of_freedom;

  //******************//
  // member functions //
  //******************//
  // Accessors for making dealing with orientation/angular velocity easier.
  inline double getYaw() const { return yawFromQuaternion(orientation_W_B); }
  inline double getYawRate() const { return angular_velocity_W.z(); }
  inline double getYawAcc() const { return angular_acceleration_W.z(); }
  // WARNING: sets roll and pitch to 0.
  inline void setFromYaw(double yaw) {
    orientation_W_B = quaternionFromYaw(yaw);
  }
  inline void setFromYawRate(double yaw_rate) {
    angular_velocity_W.x() = 0.0;
    angular_velocity_W.y() = 0.0;
    angular_velocity_W.z() = yaw_rate;
  }
  inline void setFromYawAcc(double yaw_acc) {
    angular_acceleration_W.x() = 0.0;
    angular_acceleration_W.y() = 0.0;
    angular_acceleration_W.z() = yaw_acc;
  }

  std::string toString() const {
    std::stringstream ss;
    ss << "position:          " << position_W.transpose() << std::endl
       << "velocity:          " << velocity_W.transpose() << std::endl
       << "acceleration:      " << acceleration_W.transpose() << std::endl
       << "jerk:              " << jerk_W.transpose() << std::endl
       << "snap:              " << snap_W.transpose() << std::endl
       << "yaw:               " << getYaw() << std::endl
       << "yaw_rate:          " << getYawRate() << std::endl
       << "yaw_acc:           " << getYawAcc() << std::endl;

    return ss.str();
  }
};
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenTrajectoryPoint)

// Operator overload to transform Trajectory Points according to the Eigen
// interfaces (uses operator* for this).
// Has to be outside of class.
// Example:
// Eigen::Affine3d transform; EigenTrajectoryPoint point;
// EigenTrajectoryPoint transformed = transform * point;
inline EigenTrajectoryPoint operator*(const Eigen::Affine3d& lhs,
                                      const EigenTrajectoryPoint& rhs) {
  EigenTrajectoryPoint transformed(rhs);
  transformed.position_W = lhs * rhs.position_W;
  transformed.velocity_W = lhs.rotation() * rhs.velocity_W;
  transformed.acceleration_W = lhs.rotation() * rhs.acceleration_W;
  transformed.jerk_W = lhs.rotation() * rhs.jerk_W;
  transformed.snap_W = lhs.rotation() * rhs.snap_W;
  transformed.orientation_W_B = lhs.rotation() * rhs.orientation_W_B;
  transformed.angular_velocity_W = lhs.rotation() * rhs.angular_velocity_W;
  transformed.angular_acceleration_W =
      lhs.rotation() * rhs.angular_acceleration_W;
  // Note: wrench is rotated but not transformed as an equivalent wrench.
  transformed.force_W = lhs.rotation() * rhs.force_W;
  transformed.torque_W = lhs.rotation() * rhs.torque_W;
  return transformed;
}

// eigen version of MultiDOFJointTrajectoryPoint.msg, i.e reference for multiple
// joints at a specific time
//
// EigenTrajectoryDeque/EigenTrajectoryVector contains the entire reference
// trajectory for all times and all joints in the system
struct EigenTrajectory {
  typedef std::vector<EigenTrajectory,
                      Eigen::aligned_allocator<EigenTrajectory>>
      Vector;

  //**************//
  // constructors //
  //**************//
  // default constructors
  EigenTrajectory(const int64_t& timestamp_ns_ = -1,
                  const int64_t& time_from_start_ns_ = 0)
      : timestamp_ns(timestamp_ns_), time_from_start_ns(time_from_start_ns_) {
    joints.clear();
  }

  // single joint reference constructor
  EigenTrajectory(const EigenTrajectoryPoint& point) {
    timestamp_ns = point.timestamp_ns;
    time_from_start_ns = point.time_from_start_ns;
    joints.push_back(point);
  }

  // automatic conversion from MultiJointTrajectoryPoint message
  EigenTrajectory(const MultiJointTrajectoryPoint& msg,
                  const int64_t& timestamp_ns_ = -1)
      : timestamp_ns(timestamp_ns_),
        time_from_start_ns(msg.time_from_start.toNSec()) {
    joints.clear();

    if (msg.states.empty()) {
      // outputing messages here is not nice, but before it was a ROS log,
      // which caused compilation errors if ROS wasn't included in the
      // right order in whatever other header inlucded this header.
      std::cout << "[EigenTrajectory] MultiJointTrajectoryPoint is empty." <<
          std::endl;
      return;
    }
    for (const auto& state : msg.states) {
      joints.push_back(
          EigenTrajectoryPoint(state, time_from_start_ns, timestamp_ns));
    }
  }

  //******************//
  // member variables //
  //******************//
  int64_t timestamp_ns;
  int64_t time_from_start_ns;

  EigenTrajectoryPointDeque joints;

};  // namespace mav_msgs
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenTrajectory)

struct EigenOdometry {
  EigenOdometry()
      : timestamp_ns(-1),
        position_W(Eigen::Vector3d::Zero()),
        orientation_W_B(Eigen::Quaterniond::Identity()),
        velocity_B(Eigen::Vector3d::Zero()),
        angular_velocity_B(Eigen::Vector3d::Zero()) {}

  EigenOdometry(const Eigen::Vector3d& _position,
                const Eigen::Quaterniond& _orientation,
                const Eigen::Vector3d& _velocity_body,
                const Eigen::Vector3d& _angular_velocity)
      : position_W(_position),
        orientation_W_B(_orientation),
        velocity_B(_velocity_body),
        angular_velocity_B(_angular_velocity) {}

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  int64_t
      timestamp_ns;  // Time since epoch, negative value = invalid timestamp.
  Eigen::Vector3d position_W;
  Eigen::Quaterniond orientation_W_B;
  Eigen::Vector3d velocity_B;  // Velocity in expressed in the Body frame!
  Eigen::Vector3d angular_velocity_B;
  Eigen::Matrix<double, 6, 6> pose_covariance_;
  Eigen::Matrix<double, 6, 6> twist_covariance_;

  // Accessors for making dealing with orientation/angular velocity easier.
  inline double getYaw() const { return yawFromQuaternion(orientation_W_B); }
  inline void getEulerAngles(Eigen::Vector3d* euler_angles) const {
    getEulerAnglesFromQuaternion(orientation_W_B, euler_angles);
  }
  inline double getYawRate() const { return angular_velocity_B.z(); }
  // WARNING: sets roll and pitch to 0.
  inline void setFromYaw(double yaw) {
    orientation_W_B = quaternionFromYaw(yaw);
  }
  inline void setFromYawRate(double yaw_rate) {
    angular_velocity_B.x() = 0.0;
    angular_velocity_B.y() = 0.0;
    angular_velocity_B.z() = yaw_rate;
  }

  inline Eigen::Vector3d getVelocityWorld() const {
    return orientation_W_B * velocity_B;
  }
  inline void setVelocityWorld(const Eigen::Vector3d& velocity_world) {
    velocity_B = orientation_W_B.inverse() * velocity_world;
  }
};
MAV_MSGS_MAKE_ALIGNED_CONTAINERS(EigenOdometry)

}  // namespace mav_msgs

#endif  // MAV_MSGS_EIGEN_MAV_MSGS_H
