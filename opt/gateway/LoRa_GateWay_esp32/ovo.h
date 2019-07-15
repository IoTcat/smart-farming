
/**
 * ovo-arduino Main File
 *
 * @category ovo-arduino
 * @package ovo.cpp
 * @copyright Copyright (c) 2019 EEENeko (https://github.com/eeeneko)
 * @license GNU General Public License 2.0
 * @version 0.0.1
 */


#ifndef _OVO_H
#define _OVO_H


/**
 * Like set Timeout in JS
 *
 * @Author yimian
 * @param auto function (allow Lambda)
 * @param int millisecond to dealy
 * @return void
 */
void setTimeout(void (*function)(void),const int delay)
{
    static bool on = 1;
    if(on == 1){
        static unsigned long startTime = millis(); 

        if(millis() - startTime > delay){
            (*function)();
            on == 0;
        }
    }
}

/**
 * Like set Interval in JS
 *
 * @Author yimian
 * @param auto function (allow Lambda)
 * @param int millisecond of interval
 * @return void
 */
void setInterval(void (*function)(void), const int delay)
{
    static unsigned long startTime = millis(); 

    if(millis() - startTime > delay){
        (*function)();
        startTime = millis();
    }
}





#endif

