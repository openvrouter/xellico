package.path = package.path ..";?.lua;test/?.lua;app/?.lua;"


function get_avg(arry)
	local size = #arry
	local sum = 0;
	for i=1, size, 1 do
		sum = sum + arry[i]
	end
	return sum / size
end


function setting_lat(arg_pkt_size)
	pktgen.set_range('all', 'on')
	pktgen.latency('all', 'enable')
	-- print('set range,latency mode on')

	sip_sta = '10.0.0.1'
	sip_min = '10.0.0.1'
	sip_max = '10.0.255.255'
	sip_inc = '0.0.0.1'
	pktgen.src_ip('all', 'start', sip_sta)
	pktgen.src_ip('all', 'min'  , sip_min)
	pktgen.src_ip('all', 'max'  , sip_max)
	pktgen.src_ip('all', 'inc'  , sip_inc)
	-- printf('src.ip %s [%s to %s] inc:%s\n',
		-- sip_sta, sip_min, sip_max, sip_inc)

	dip_sta = '10.10.0.1'
	dip_min = '10.10.0.1'
	dip_max = '10.10.255.255'
	dip_inc = '0.0.0.1'
	pktgen.dst_ip('all', 'start', dip_sta)
	pktgen.dst_ip('all', 'min'  , dip_min)
	pktgen.dst_ip('all', 'max'  , dip_max)
	pktgen.dst_ip('all', 'inc'  , dip_inc)
	-- printf('dst.ip %s [%s to %s] inc:%s\n',
	-- 	dip_sta, dip_min, dip_max, dip_inc)

	pktsize_sta = arg_pkt_size
	pktsize_min = 0
	pktsize_max = 0
	pktsize_inc = 0
	pktgen.pkt_size('all', 'start', pktsize_sta);
	pktgen.pkt_size('all', 'min'  , pktsize_min);
	pktgen.pkt_size('all', 'max'  , pktsize_max);
	pktgen.pkt_size('all', 'inc'  , pktsize_inc);
	-- printf('pkt_size %d [%d to %d] inc:%d\n',
	-- 	pktsize_sta, pktsize_min, pktsize_max, pktsize_inc)
	return
end


function setting_tp(arg_pkt_size)
	pktgen.set_range('all', 'on')
	pktgen.latency('all', 'disable')
	-- print('set range mode on')

	sip_sta = '10.0.0.1'
	sip_min = '10.0.0.1'
	sip_max = '10.0.255.255'
	sip_inc = '0.0.0.1'
	pktgen.src_ip('all', 'start', sip_sta)
	pktgen.src_ip('all', 'min'  , sip_min)
	pktgen.src_ip('all', 'max'  , sip_max)
	pktgen.src_ip('all', 'inc'  , sip_inc)
	-- printf('src.ip %s [%s to %s] inc:%s\n',
		-- sip_sta, sip_min, sip_max, sip_inc)

	dip_sta = '10.10.0.1'
	dip_min = '10.10.0.1'
	dip_max = '10.10.255.255'
	dip_inc = '0.0.0.1'
	pktgen.dst_ip('all', 'start', dip_sta)
	pktgen.dst_ip('all', 'min'  , dip_min)
	pktgen.dst_ip('all', 'max'  , dip_max)
	pktgen.dst_ip('all', 'inc'  , dip_inc)
	-- printf('dst.ip %s [%s to %s] inc:%s\n',
	-- 	dip_sta, dip_min, dip_max, dip_inc)

	pktsize_sta = arg_pkt_size
	pktsize_min = 0
	pktsize_max = 0
	pktsize_inc = 0
	pktgen.pkt_size('all', 'start', pktsize_sta);
	pktgen.pkt_size('all', 'min'  , pktsize_min);
	pktgen.pkt_size('all', 'max'  , pktsize_max);
	pktgen.pkt_size('all', 'inc'  , pktsize_inc);
	-- printf('pkt_size %d [%d to %d] inc:%d\n',
	-- 	pktsize_sta, pktsize_min, pktsize_max, pktsize_inc)
	return
end


function test_tp(delay_msec, cnt, key)
	pktgen.clr();
	pktgen.start('all');
	local rx = {}
	pktgen.delay(2000)
	for i=1, cnt, 1 do
		local portStats = pktgen.portStats("all", "rate");
		local cur = portStats[0][key] + portStats[1][key]

		table.insert(rx, cur)
		-- printf("%s:%d, \n", key, cur)
		pktgen.delay(delay_msec)
	end
	pktgen.stop('all');
	return rx
end


function test_lat(delay_msec, cnt)
	pktgen.clr();
	pktgen.start('all');
	local rx = {}
	pktgen.delay(2000)
	for i=1, cnt, 1 do
		local pktStats  = pktgen.pktStats("all")
		local cur = pktStats[0]['avg_latency']

		table.insert(rx, cur)
		-- printf("%s:%d, \n", key, cur)
		pktgen.delay(delay_msec)
	end
	pktgen.stop('all');
	return rx
end


function main()
	local cnt = 5

	setting_lat(64  ); local lat64   = math.floor(get_avg(test_lat(1000, cnt)));
	setting_lat(128 ); local lat128  = math.floor(get_avg(test_lat(1000, cnt)));
	setting_lat(192 ); local lat192  = math.floor(get_avg(test_lat(1000, cnt)));
	setting_lat(256 ); local lat256  = math.floor(get_avg(test_lat(1000, cnt)));
	setting_lat(512 ); local lat512  = math.floor(get_avg(test_lat(1000, cnt)));
	setting_lat(1024); local lat1024 = math.floor(get_avg(test_lat(1000, cnt)));
	setting_lat(1514); local lat1514 = math.floor(get_avg(test_lat(1000, cnt)));

	setting_tp(64  ); local bps64   = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));
	setting_tp(128 ); local bps128  = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));
	setting_tp(192 ); local bps192  = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));
	setting_tp(256 ); local bps256  = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));
	setting_tp(512 ); local bps512  = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));
	setting_tp(1024); local bps1024 = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));
	setting_tp(1514); local bps1514 = math.floor(get_avg(test_tp(1000, cnt, 'mbits_rx')));

	setting_tp(64  ); local pps64   = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));
	setting_tp(128 ); local pps128  = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));
	setting_tp(192 ); local pps192  = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));
	setting_tp(256 ); local pps256  = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));
	setting_tp(512 ); local pps512  = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));
	setting_tp(1024); local pps1024 = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));
	setting_tp(1514); local pps1514 = math.floor(get_avg(test_tp(1000, cnt, 'pkts_rx')));

	printf("%10d, %10d, %10d, %10d, %10d, %10d, %10d,  [usec]\n",
		lat64, lat128, lat192, lat256, lat512, lat1024, lat1514);
	printf("%10d, %10d, %10d, %10d, %10d, %10d, %10d,  [Mbps]\n",
		bps64, bps128, bps192, bps256, bps512, bps1024, bps1514);
	printf("%10d, %10d, %10d, %10d, %10d, %10d, %10d,  [pps]\n",
		pps64, pps128, pps192, pps256, pps512, pps1024, pps1514);
end

main()
