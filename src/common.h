#ifndef __GBTREE__COMMON
#define __GBTREE__COMMON

#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <time.h>
#include <stdint.h>

#include <ext/hash_set>
#include <ext/hash_map>

#include <boost/algorithm/string.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/device/file.hpp> 
#include <boost/functional/hash.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/timer.hpp>
#include <boost/bind.hpp>

#define MINLOG -50.0
#define MAXLOG 50.0 

#define MINETA 1e-5

const double MAXEXP = exp(MAXLOG);
const double MINEXP = exp(MINLOG);
static inline double minmaxexp(double x) {
    return x<MINEXP?MINEXP:(x>MAXEXP?MAXEXP:x);
}

const double eps=1e-8;
static inline int dcmp(double x) {
	return x<-eps?-1:x>eps;
}

static inline double trlog(double x) {
	double	tmp = log(x);
	return tmp<MINLOG?MINLOG:tmp;
}

template <typename T>
static inline T sqr(T a) {
	return a*a;
}

template <class T>
inline bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&) = std::dec)
{
	std::istringstream iss(s);  
	return (iss >> f >> t);
}

template <class T>
inline bool to_string(const T& t, std::string& s, std::ios_base& (*f)(std::ios_base&) = std::dec)
{
	std::ostringstream oss;  
	bool ret = (oss << f << t);
	s = oss.str();
	return ret;
}

#if 0
typedef flens::GeMatrix<flens::FullStorage<double, flens::ColMajor> > DGeMatrix;
typedef flens::SyMatrix<flens::FullStorage<double, flens::ColMajor> > DSyMatrix;
typedef flens::DenseVector<flens::Array<double> > DDenseVector;
typedef flens::DenseVector<flens::Array<int> > IDenseVector;
#endif

typedef std::vector<double> doubles_t;
typedef std::vector<int> ints_t;
typedef std::vector<uint64_t> uint64s_t;
typedef std::vector<std::string> strings_t;
typedef __gnu_cxx::hash_set<int> intset_t;
typedef __gnu_cxx::hash_map<int, int> int2int_t;
typedef __gnu_cxx::hash_map<int, double> int2double_t;
typedef __gnu_cxx::hash_map<int, std::string> int2str_t;

typedef std::vector<size_t> sizes_t;
typedef std::pair<int, int> int_pair;
typedef std::vector<int_pair> int_pairs_t;
typedef __gnu_cxx::hash_map<int_pair, int, boost::hash<int_pair> > intp2int_t;
typedef __gnu_cxx::hash_map<int_pair, double, boost::hash<int_pair> > intp2double_t;

template <typename RandomAccessIter, typename LessThan>
struct CompareKey {
	LessThan lessThan;
	RandomAccessIter s;
	CompareKey(RandomAccessIter first) : s(first) {}
	bool operator()(const size_t a, const size_t b) const {
		return lessThan(*(s+a), *(s+b));
	}
};

template <typename RandomAccessIter, typename LessThan>
inline void sortArg(RandomAccessIter s, RandomAccessIter t, sizes_t& ind, LessThan lessThan) {
	ind.resize(t-s);
	for(size_t i=0;i<ind.size();i++) ind[i] = i;
	sort(ind.begin(), ind.end(), CompareKey<RandomAccessIter, LessThan>(s));
}


inline void build_stream(boost::iostreams::filtering_istream& stream,
	const std::string& file) {
		if(boost::ends_with(file, ".gz")  || boost::ends_with(file, ".bz2")) {
			//    file_stream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
			if (boost::ends_with(file,".gz")) {
				stream.push(boost::iostreams::gzip_decompressor());
			} 
			if(boost::ends_with(file, ".bz2")) {
				stream.push(boost::iostreams::bzip2_decompressor());    
			}
			stream.push(boost::iostreams::file_source(file, std::ios_base::in|std::ios_base::binary));
		} else {
			stream.push(boost::iostreams::file_source(file));
		}
}

inline void build_stream(boost::iostreams::filtering_ostream& stream,
	const std::string& file) 
{
	if(boost::ends_with(file, ".gz")  || boost::ends_with(file, ".bz2")) {
		//    file_stream.open(file.c_str(), std::ios_base::out|std::ios_base::binary);
		//    file_stream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
		if (boost::ends_with(file,".gz")) {
			stream.push(boost::iostreams::gzip_compressor());
		} 
		if(boost::ends_with(file, ".bz2")) {
			stream.push(boost::iostreams::bzip2_compressor());    
		}
		stream.push(boost::iostreams::file_sink(file, std::ios_base::out|std::ios_base::binary));
	} else {
		stream.push(boost::iostreams::file_sink(file));
	}
}


int myclock ()
{
	time_t seconds;
	seconds = time (NULL);
	return seconds;
}

char *
rtime (double t)
{
	int hour, min, sec;
	static char buf[BUFSIZ];

	hour = ((int)floor(t)) / 3600;
	min  = ((int)floor(t) % (60 * 60)) / 60;
	sec  = (int)floor(t) % 60;
	sprintf(buf, "%2d:%02d:%02d", hour, min, sec);
	
	return buf;
}

void pause(){
    printf("Pausing the program, type any character to restart...\n");
    getchar();
}

#endif
