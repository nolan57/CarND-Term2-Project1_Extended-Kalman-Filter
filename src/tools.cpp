#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
	VectorXd rmse(4);
	rmse << 0, 0, 0, 0;

	// check the validity of the following inputs:
	//  * the estimation vector size should not be zero
	//  * the estimation vector size should equal ground truth vector size
	if (estimations.size() != ground_truth.size()
		|| estimations.size() == 0) {
		cout << "Invalid estimation or ground_truth data" << endl;
		return rmse;
	}

	//accumulate squared residuals
	for (unsigned int i = 0; i < estimations.size(); ++i) {

		VectorXd residual = estimations[i] - ground_truth[i];

		//coefficient-wise multiplication
		residual = residual.array()*residual.array();
		rmse += residual;
	}

	//calculate the mean
	rmse = rmse / estimations.size();

	//calculate the squared root
	rmse = rmse.array().sqrt();
	//return the result
	return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
  TODO:
    * Calculate a Jacobian here.
  */
	MatrixXd Hj(3, 4);
	//recover state parameters
	float px = x_state(0);
	float py = x_state(1);
	float vx = x_state(2);
	float vy = x_state(3);
	//pre-compute a set of terms to avoid repeated calculation
	float c1 = px*px + py*py;
	//check division by zero
	if (fabs(c1) < 0.0001) {
		cout << "CalculateJacobian () - Error - Division by Zero" << endl;
		c1=0.0001;
	}
	float c2 = sqrt(c1);
	float c3 = (c1*c2);//(px^2 + py^2)^(3/2)
	float vxpy = vx*py;
	float vypx = vy*px;
	float px_over_c2 = px / c2;
	float py_over_c2 = py / c2;
	//compute the Jacobian matrix
	Hj << px_over_c2, py_over_c2, 0, 0,
		-(py / c1), (px / c1), 0, 0,
		py*(vxpy - vypx) / c3, px*(vypx - vxpy) / c3, px_over_c2, py_over_c2;
	
	return Hj;
}

/**
* Convert radar from polar to cartesian coordinates
*/
VectorXd Tools::ConvertPolarToCartesian(VectorXd x_measurements) {
	float ro = x_measurements[0];//rho , range
	float theta = x_measurements[1];//phi , bearing
	
	float px = ro * cos(theta);
	float py = ro * sin(theta);
	
	VectorXd converted_raw_measurements_ = VectorXd(4);
	converted_raw_measurements_ << px, py,0,0;
	return converted_raw_measurements_;
}

