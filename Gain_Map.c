static unsigned int _cmos_gains_convert(unsigned int a_gain, unsigned int d_gain,
		unsigned int *a_gain_out ,unsigned int *d_gain_out)
{
    //?¡ê??????????¡Áa????????????¡¤??????????¡Áa????sensor?????????
    #define _2XGAIN	2048
	unsigned int  fine_gain = 0;
	int tmp = 0;
	//int ret = 0;

	//ak_sensor_print("a_gain =%d\n",a_gain);

    if (a_gain == g_a_gain)
		return 0;
	
    g_a_gain = a_gain;

	if(a_gain<_2XGAIN)       // 1 -1.9375
	{
		tmp = a_gain-1024;
		hm2140_curr_again_level = 1;
    }
    else if(a_gain<4096)       // 2 - 3.875
    {
		tmp = a_gain/2-1024;
		hm2140_curr_again_level = 2;
    }
    else if(a_gain<8192)       //4-7.75
    {
		tmp = a_gain/4-1024;
		hm2140_curr_again_level = 4;
    }
    else if(a_gain<16384)       //8 - 15.5
    {
		tmp = a_gain/8-1024;
		hm2140_curr_again_level = 8;
    }
    
    if(tmp >= 0){
		fine_gain = (tmp/64)*hm2140_curr_again_level; //Calculate Gain Map's X low four bit
	}
	else{			
		fine_gain = 0;
	}

	fine_gain &= 0x0f;

	if ((a_gain/1024-1) >= 0) {
        fine_gain |= ((a_gain/1024-1)<<4);// Calculate Gain Map's X high four bit
	}

	//printk(KERN_ERR"again:%d, fine_gain:%x******************************\n",a_gain>>2, fine_gain);
	hm2140_sensor_write_register(0x0205, fine_gain);
	ak_sensor_mdelay(10);
	hm2140_sensor_write_register(0x0104,0x1); //update 
	return 0;
}

static int hm2140_cmos_updata_a_gain(const unsigned int a_gain)
{
	unsigned int tmp_a_gain;
	unsigned int tmp_d_gain;
	unsigned int tmp_a_gain_out;
	unsigned int tmp_d_gain_out;

	tmp_a_gain = a_gain<<2;
	tmp_d_gain = 0;

	return _cmos_gains_convert(tmp_a_gain, tmp_d_gain,  &tmp_a_gain_out ,&tmp_d_gain_out);
}