//    Copyright 2017 Rainer Gemulla
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
#ifndef MF_SGD_FUNCTIONS_UPDATE_NZSLL_H
#define MF_SGD_FUNCTIONS_UPDATE_NZSLL_H

#include <boost/serialization/serialization.hpp>

#include <mpi2/types.h>
#include <mpi2/uninitialized.h>

#include <mf/sgd/functions/functions.h>
#include <mf/matrix/distributed_matrix.h>
#include <mf/sgd/sgd.h>
#include <mf/types.h>


namespace mf {

struct UpdateSl : public UpdateConcept {
	UpdateSl() { };
	UpdateSl(mpi2::SerializationConstructor _) { };

	inline void operator()(FactorizationData<>& data,
			const unsigned int i, const unsigned int j,	const double x,
			const double eps) {
		mf_size_type ir = i*data.r;
		mf_size_type jr = j*data.r;
		double f = eps * -2. * x;
		for (unsigned z=0; z<data.r; z++) {
			double temp = data.wValues[ir + z];
			data.wValues[ir + z] -= f * data.hValues[z + jr];
			data.hValues[z + jr] -= f * temp;
		}
;
	}

private:
	friend class ::boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	}
};

}

MPI2_TYPE_TRAITS(mf::UpdateSl);
MPI2_SERIALIZATION_CONSTRUCTOR(mf::UpdateSl);

#endif
