package.path = package.path ..";?.lua;test/?.lua;app/?.lua;"

function setting(arg_pkt_size)
	pktgen.set_range('all', 'on')
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


function get_avg(arry, size)
	local sum = 0;
	for i=1, size, 1 do
		sum = sum + arry[i]
	end
	return sum / size
end


function test(delay_msec, cnt, key)
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


function main()
	local cnt = 5
	local dum = 0
	local key = 'mbits_rx' -- pkts_rx,mbits_rx,avg_latency

	setting(64  ); local res64   = math.floor(get_avg(test(1000, cnt, key), cnt));
	setting(128 ); local res128  = math.floor(get_avg(test(1000, cnt, key), cnt));
	setting(192 ); local res192  = math.floor(get_avg(test(1000, cnt, key), cnt));
	setting(256 ); local res256  = math.floor(get_avg(test(1000, cnt, key), cnt));
	setting(512 ); local res512  = math.floor(get_avg(test(1000, cnt, key), cnt));
	setting(1024); local res1024 = math.floor(get_avg(test(1000, cnt, key), cnt));
	setting(1514); local res1514 = math.floor(get_avg(test(1000, cnt, key), cnt));

	printf("%10d, ", res64  )
	printf("%10d, ", res128 )
	printf("%10d, ", res192 )
	printf("%10d, ", res256 )
	printf("%10d, ", res512 )
	printf("%10d, ", res1024)
	printf("%10d, ", res1514)
	printf("\n")
end


main()

