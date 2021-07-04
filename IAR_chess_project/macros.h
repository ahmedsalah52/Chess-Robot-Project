/*
 * macros.h
 *
 *  Created on: Aug 11, 2018
 *      Author: Ahmatronics
 */

#ifndef MACROS_H_
#define MACROS_H_

#define SET_BIT(Reg,Pin)     Reg|=(1<<Pin)   //OR GATE WITH THE BIT WE WANT TO CHANGE TO BE CHANGED IN THE REG
#define CLEAR_BIT(Reg,Pin)   Reg&=~(1<<Pin)  //AND GATE TO WITH ZERO TO CLEAR THE BIT
#define TOGGLE_BIT(Reg,Pin)  Reg^=(1<<Pin)   //XOR WITH THE BITS IN THE REG TO TOGGLE THE VALUE
#define GET_BIT(Reg,Pin)     ((Reg>>Pin)&1)

#endif /* MACROS_H_ */
