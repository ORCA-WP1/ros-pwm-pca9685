/* pca9685_i2c_node.cpp
 * Author: Dheera Venkatraman <dheera@dheera.net>
 *
 * Instantiates a PCA9685 Activity class, as well as
 * a Watchdog that causes this node to die if things aren't
 * working.
 */

#include <pwm_pca9685/pca9685_activity.h>
#include <csignal>

int main(int argc, char *argv[]) {
    ros::NodeHandle* nh = NULL;
    ros::NodeHandle* nh_priv = NULL;
    double min_pwm, max_pwm, timeout_value,timeout, frequency, pwm_offset = 0;
    pwm_pca9685::PCA9685Activity* activity = NULL;

    ros::init(argc, argv, "pca9685_node");

    nh = new ros::NodeHandle();
    if(!nh) {
        ROS_FATAL("Failed to initialize NodeHandle");
        ros::shutdown();
        return -1;
    }

    nh_priv = new ros::NodeHandle("~");
    if(!nh_priv) {
        ROS_FATAL("Failed to initialize private NodeHandle");
        delete nh;
        ros::shutdown();
        return -2;
    }


    if (!nh_priv->getParam("min_pwm", min_pwm)){
        ROS_FATAL("Failed to load min_pwm param");
    }

    if (!nh_priv->getParam("max_pwm", max_pwm)){
        ROS_FATAL("Failed to load max_pwm param");
    }

    if (!nh_priv->getParam("timeout", timeout)){
        ROS_FATAL("Failed to load timeout param");
    }

    if (!nh_priv->getParam("timeout_value", timeout_value)){
        ROS_FATAL("Failed to load timeout_value param");
    }

    if (!nh_priv->getParam("frequency", frequency)){
        ROS_FATAL("Failed to load frequency param");
    }

    if (!nh_priv->getParam("pwm_offset", pwm_offset)){
        ROS_FATAL("Failed to load pwm_offset param");
    }

    min_pwm+=pwm_offset;
    max_pwm+=pwm_offset;
    timeout_value+=pwm_offset;
    activity = new pwm_pca9685::PCA9685Activity(*nh, *nh_priv, min_pwm, max_pwm, timeout, timeout_value, frequency);

    if(!activity) {
        ROS_FATAL("Failed to initialize driver");
        delete nh_priv;
        delete nh;
        ros::shutdown();
        return -3;
    }

    if(!activity->start()) {
        ROS_ERROR("Failed to start activity");
        delete nh_priv;
        delete nh;
        ros::shutdown();
        return -4;
    }

    ros::Rate rate(100);
    while(ros::ok()) {
        rate.sleep();
        activity->spinOnce();
    }

    activity->stop();

    delete activity;
    delete nh_priv;
    delete nh;

    return 0;
}
