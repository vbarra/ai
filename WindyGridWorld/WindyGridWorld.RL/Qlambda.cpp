#include "Qlambda.h"

using namespace native::alg;

QLambda::QLambda(double alpha, double gamma, double lambda):
	frw::IAlgorithm(alpha, gamma), lambda_(lambda)
{
}

void QLambda::DoOneLearningIterate() {

	env_->ResetAgent();
	int s0 = env_->GetCurrentAsIndex();
	int a0 = 0;
	ActionSelectors::EpsilonGreedy(max(s0), &a0);

	int inner_loop = 0;

	while (!env_->IsTerminated() && inner_loop < 100) {

		env_->ExecuteAction(a0);
		double reward = env_->RewardFunc();
		int s1 = env_->GetCurrentAsIndex();

		int a_str = 0;
		int a1 = max(s1);
		ActionSelectors::EpsilonGreedy(a1, &a_str);

		float delta = reward + gamma_ * qsa_[s1][a_str] - qsa_[s0][a0];
		zsa_[s0][a0] = zsa_[s0][a0] + 1;

		for (int s = 0; s < env_->GetValuesNumber(); ++s) {
			for (int a = 0; a < 4; ++a) {

				qsa_[s][a] = qsa_[s][a] + alpha_ * delta * zsa_[s][a];

				if(a_str == a1)
					zsa_[s][a] = zsa_[s][a] * gamma_ * lambda_;
				else
					zsa_[s][a] = 0;
			}
		}

		a0 = a1;
		s0 = s1;
		inner_loop += 1;
	}
}

void QLambda::SetEnvironment(native::frw::Environment * env) {

	env_ = env;
	int state_sz = env_->GetValuesNumber();

	qsa_.resize(state_sz);
	zsa_.resize(state_sz);

	for (int i = 0; i < state_sz; ++i) {
		qsa_[i].resize(4); // four actions are possible
		zsa_[i].resize(4);
	}

	for (int i = 0; i < state_sz; ++i) {
		for (int j = 0; j < 4; ++j) {
			qsa_[i][j] = 0.0;

		}
	}
}