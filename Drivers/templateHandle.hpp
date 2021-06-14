/*
 * Handle.hpp
 *
 *  Created on: Jun 14, 2021
 *      Author: Administrator
 */

#ifndef DRIVERS_TEMPLATEHANDLE_HPP_
#define DRIVERS_TEMPLATEHANDLE_HPP_



template <typename T,int MAX_HANDLES_NUM>
T** handleRegister(T *handle){
	static T* classes[MAX_HANDLES_NUM] = {0};
	for(int i = 0; i < MAX_HANDLES_NUM ;i++){
		if(classes[i] == NULL){
			classes[i] = handle;
			break;
		}
	}
	return classes;
}

template <typename C, typename H>
C getClass(C* classes, H *handle,int MAX_CLASS_NUM){
	C _class = nullptr;
	for(int i = 0; (classes[i] != NULL)&& (i < MAX_CLASS_NUM); ++i){
		if(classes[i]->getHandle()->Instance == handle->Instance){
			_class = classes[i];
		}
	}
	return _class;
}


#endif /* DRIVERS_TEMPLATEHANDLE_HPP_ */
