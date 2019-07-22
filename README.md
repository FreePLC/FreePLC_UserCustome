# FreePLC_UserCustome
本工程为FreePLC项目的子工程，主要用于提供PLC IO通道工程模板，支持16路数字量输入，16路数字量输出，8路模拟量输入，2路PTO，2路HCI，用户可以根据自身需求，定制外设。
##plc_usercode.c相关函数API说明：
plc_user_init:用户通道初始化函数，上电运行一次<br>
plc_user_main:用户主循环调用函数，上电后每个运算周期调用一次。<br>
plc_init_dio:通道初始化函数，PLC工程中的通道（类似%IB）与用户函数通过一片内存区进行交互，内存区分为I（输入）,Q（输出）,M（读写）。初始化函数主要是从内存区获取通道的数据地址<br>
plc_get_input:从硬件端口获取输入数据，并更新到I区<br>
plc_set_output:从Q区获取输出数据，并更新到物理输出端口<br>
##plc_main.c相关API说明：
该函数提供了PLC运行时间的tick以及R区（掉电保持区）的操作，PLC用户会存储一些标定信息或者配置参数，本示例使用KV4x内部的Flash做为R区，需要注意，芯片内部的Flash有擦写次数的限制（通常5~10w）次，如果用户需要频繁操作R区，建议外部使用MRAM或者FRAM <br>
void PLC_GetTime(IEC_TIME *CURRENT_TIME) //获取运行时间 <br>
void PLC_SetTimer( uint64_t tick_next, uint64_t tick_period ) //设置运行时间 <br>
int plc_backup_check(void)  //上电时检查R区，如果校验成功则恢复到R区 <br>
void plc_backup_validate(void)  //当R区内容发生变化时，更新到Flash中 <br>

总结：
如果用户不频繁更新R区数据，则仅需要修改plc_usercode.c中相关的函数，根据自己的硬件设计添加IO的初始化已经通道获取函数。用户可以各个外设的中断服务，工程已经做好映射，可以参考OpenModbus相关UART的中断配置。
