/* exceptions.c - Implements the various idt exception handlers
 * vim:ts=4 noexpandtab
 */

#include "rtc_driver.h"

rtc_driver_instance_t rtc_drivers_instances[MAX_RTC_DRIVERS];

/*  Description: Opens an rtc driver and finds the first open instance for the calling process to use
 *  Input: A pointer to an integer
 *  Output: 0 if successful and -1 if not(no open instances)
 *  Effect: Sets the first open instance to in_use and puts the index of that instance in the given pointer
 * 		Also sets the rtc interrupt rate to 1024Hz
 */
int rtc_open(const uint8_t* fname, uint32_t fd)
{	
int pid = get_pid();
    if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();

	int first_open = 0;
	
	set_rtc_rate(TEN_ZERO_TWENTY_FOUR_HZ_RATE);

	//searches through the instance for the first not in use
	for(first_open = 0;first_open < MAX_RTC_DRIVERS;first_open++)
	{
		if(rtc_drivers_instances[first_open].in_use == 0)
		{
			//puts this instance to use and gives the caller the instance index
			rtc_drivers_instances[first_open].in_use = 1;
			processes[pid]->file_descriptors[fd].position = first_open;
			return 0;

		}
	}
	return -1;
}

/*  Description: Waits for the next interrupt for this instance, then returns
 *  Input: An rtc driver instance index
 *  Output: 0 if successful and -1 if not(invalid instance index)
 *  Effect: None
 */
int rtc_read(uint32_t fd, uint8_t* buf, int32_t nbytes)
{
int pid = get_pid();
	if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();
	uint32_t rtc_index = processes[pid]->file_descriptors[fd].position;

	//check if the rtc index is valid
	if(rtc_index >= MAX_RTC_DRIVERS || rtc_drivers_instances[rtc_index].in_use == 0)
	{		
		return -1;
	}

	//Raise this instance's flag, then wait for the interrupt handler to trigger it 
	rtc_drivers_instances[rtc_index].flag = 1;
	while(rtc_drivers_instances[rtc_index].flag == 1)
	{

	}
	return 0;

}

/*  Description: Sets the interrupt frequency for this instance
 *  Input: An rtc driver instance index and a frequency interrupt rate
 *  Output: 0 if successful and -1 if not(invalid instance index/invalid frequency)
 *  Effect: None
 */
int rtc_write(uint32_t fd, const uint8_t* buf, int32_t nbytese)
{
int pid = get_pid();
if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();
	uint32_t rtc_index = processes[pid]->file_descriptors[fd].position;
	//check if the rtc instance index is valid
	if(rtc_index >= MAX_RTC_DRIVERS || rtc_drivers_instances[rtc_index].in_use == 0)
	{		
		return -1;
	}
	if(buf == 0)
	{
		return -1;
	}
	//check if the rate is valid(not zero, less then or equal to 1024, power of 2
	if((*buf & (*buf-1)) != 0 || *buf > MAX_RTC_FREQUENCY || *buf <= 0)
	{
		return -1;
	}

	//sets the amount of ticks the interrupt handler should wait between each interrupt for this instance
	rtc_drivers_instances[rtc_index].wait_ticks = MAX_RTC_FREQUENCY / *buf;

	return 0;
}

/*  Description: Closes the given rtc index
 *  Input: An rtc driver instance index
 *  Output: 0 if successful and -1 if not(invalid instance index)
 *  Effect: None
 */
int rtc_close(uint32_t fd)
{
int pid = get_pid();
	if (pid < 0 || pid >= NUMBER_OF_PROCESSES) //if the current process id is out of bounds return -1
    return -1;
    if(fd < 0 || fd >= NUMBER_OF_FILE_DESCRIPTORS)
    return -1;
    pcb_t** processes = get_process();
	//check if the rtc instance index is valid
	if(processes[pid]->file_descriptors[fd].position >= MAX_RTC_DRIVERS || rtc_drivers_instances[processes[pid]->file_descriptors[fd].position].in_use == 0)
	{		
		return -1;
	}
	//Sets this instance to not in use
	rtc_drivers_instances[processes[pid]->file_descriptors[fd].position].in_use = 0;
	return 0;
}


