#ifndef __HAWKES_H__
#define __HAWKES_H__

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <utility>

typedef std::pair<int, double> intdouble_t;
typedef std::vector<intdouble_t> intdoubles_t;
typedef std::vector<std::pair<int,int> > words_t;
struct Event {
	int mid;
	int uid;
	double t;
	words_t words;
	Event(int mm=0, int uu=0, double tt=0):uid(uu), mid(mm), t(tt) {}
	friend std::ostream& operator<< (std::ostream &out, const Event& e);
};


std::ostream& operator<< (std::ostream &out, const Event& e){
	out << e.mid << "\t" << e.uid << "\t" << e.t;
	out << "\t" << e.words.size();
	for(size_t k=0; k<e.words.size(); k++) 
		out << " " << e.words[k].first << ":" << e.words[k].second;
	return out;
}

typedef std::vector<Event> Events;


struct HawkesModel {
	double mu, alpha, w;

	HawkesModel(double m=0.0, double a=0.0, double ww=0.0) : mu(m), alpha(a), w(ww) {}


	friend std::ostream& operator<< (std::ostream &out, const HawkesModel& model);

};

std::ostream& operator<< (std::ostream &out, const HawkesModel& model) {
	out << model.mu << "\t" << model.alpha << "\t" << model.w;
	return out;
}

struct MultiHawkesModel {
	int D;
	doubles_t alpha;
	double mu;

	MultiHawkesModel(int dd=0) : D(dd), alpha(dd*dd), mu(0.0) {}

	friend std::ostream& operator<< (std::ostream &out, const MultiHawkesModel& model);  
	friend std::istream& operator>> (std::istream &in, MultiHawkesModel& model);
};

std::ostream& operator<< (std::ostream &out, const MultiHawkesModel& model) {
	out << model.mu << "\t" << model.D;
	for(size_t i=0;i<model.alpha.size();i++) out << "\t" << model.alpha[i];  
	return out;
}

std::istream& operator>> (std::istream &in, MultiHawkesModel& model) {
	in >> model.mu >> model.D;
	model.alpha.resize(model.D*model.D);
	for(size_t i=0;i<model.alpha.size();i++) in >> model.alpha[i];
	return in;
}


inline double g(double dt, double W=1.0) {
	return minmaxexp(W*exp(-W*dt));
}

inline double G(double dt, double W=1.0) {
	return (1.0 - minmaxexp(exp(-W*dt)));
}

inline double intensity(const MultiHawkesModel& model, const intdoubles_t& e, double t, int d) {
	double res = model.mu;
	int pd = d*model.D;
	for(size_t i=0;i<e.size();i++) {
		res += model.alpha[pd+e[i].first]*g(t-e[i].second);
	}
	return res;
}

struct MixedHawkesModel {
	int D, M, V;
	doubles_t alpha, mu, p, beta, gamma;

	MixedHawkesModel(int dd=0, int mm=0, int vv=0) : D(dd), alpha(dd*dd), mu(dd), M(mm), p(mm), V(vv), beta(mm*vv), gamma(mm) {}

	friend std::ostream& operator<< (std::ostream &out, const MixedHawkesModel& model);  
	friend std::istream& operator>> (std::istream &in, MixedHawkesModel& model);
};

std::ostream& operator<< (std::ostream &out, const MixedHawkesModel& model) {
	out << model.D << "\t" << model.M << "\t" << model.V << std::endl;
	for(size_t i=0;i<model.mu.size();i++) out << "\t" << model.mu[i];
	out  << std::endl; 
	for(size_t i=0;i<model.gamma.size();i++) out << "\t" << model.gamma[i];
	out  << std::endl; 
	for(size_t i=0;i<model.alpha.size();i++) out << "\t" << model.alpha[i];
	out << std::endl;     
	for(size_t i=0;i<model.p.size();i++) out << "\t" << model.p[i];
	out << std::endl;  
	for(size_t i=0;i<model.beta.size();i++) out << "\t" << model.beta[i];  
	return out;
}

std::istream& operator>> (std::istream &in, MixedHawkesModel& model) {
	in >> model.D >> model.M >> model.V;
	model.mu.resize(model.D);
	for(size_t i=0;i<model.mu.size();i++) in >> model.mu[i];
	model.gamma.resize(model.M);
	for(size_t i=0;i<model.gamma.size();i++) in >> model.gamma[i];
	model.alpha.resize(model.D*model.D);
	for(size_t i=0;i<model.alpha.size();i++) in >> model.alpha[i];
	model.p.resize(model.M);
	for(size_t i=0;i<model.p.size();i++) in >> model.p[i];
	model.beta.resize(model.V*model.M);
	for(size_t i=0;i<model.beta.size();i++) in >> model.beta[i];
	return in;
}

inline double mintensity(const MixedHawkesModel& model, const Events& e, double t, int d, int m) {
	double res = model.mu[d]*model.gamma[m];
	int pd = d*model.D;
	for(size_t i=0;i<e.size();i++) {
		res +=  model.alpha[pd+e[i].uid]*g(t-e[i].t);
	}
	return res;
}

#endif
