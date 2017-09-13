/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */



#ifndef DRX_PARAMETER_H_
#define DRX_PARAMETER_H_
#include <stdint.h>

#define DRX_PARAMETER_MINIMUM_LENGTH 3
#define DRX_PARAMETER_MAXIMUM_LENGTH 3

struct DrxParameter_tag {
  uint8_t  splitpgcyclecode;
  uint8_t  cnspecificdrxcyclelengthcoefficientanddrxvaluefors1mode:4;
  uint8_t  splitonccch:1;
  uint8_t  nondrxtimer:3;
} ;

class DrxParameter{
	
public:
	DrxParameter(DrxParameter_tag _drxparameter;
	~DrxParameter();
			
	int encode(uint8_t iei, uint8_t *buffer, uint32_t len);
	int decode(uint8_t iei, uint8_t *buffer, uint32_t len);
	void dump(uint8_t iei);
	uint8_t encode_u8();
	int decode_u8(uint8_t iei, uint8_t value, uint32_t len);
	
private:
	DrxParameter_tag drxparameter;
	
};

#endif /* DRX PARAMETER_H_ */


