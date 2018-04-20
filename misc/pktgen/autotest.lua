package.path = package.path ..";?.lua;test/?.lua;app/?.lua;"

function setting(arg_pkt_size)
	pktgen.set_range('all', 'on')

	print('set range mode on')
	sip_sta = '10.0.0.1'
	sip_min = '10.0.0.1'
	sip_max = '10.0.255.255'
	sip_inc = '0.0.0.1'
	pktgen.src_ip('all', 'start', sip_sta)
	pktgen.src_ip('all', 'min'  , sip_min)
	pktgen.src_ip('all', 'max'  , sip_max)
	pktgen.src_ip('all', 'inc'  , sip_inc)
	printf('src.ip %s [%s to %s] inc:%s\n',
		sip_sta, sip_min, sip_max, sip_inc)

	dip_sta = '10.10.0.1'
	dip_min = '10.10.0.1'
	dip_max = '10.10.255.255'
	dip_inc = '0.0.0.1'
	pktgen.dst_ip('all', 'start', dip_sta)
	pktgen.dst_ip('all', 'min'  , dip_min)
	pktgen.dst_ip('all', 'max'  , dip_max)
	pktgen.dst_ip('all', 'inc'  , dip_inc)
	printf('dst.ip %s [%s to %s] inc:%s\n',
		dip_sta, dip_min, dip_max, dip_inc)

	pktsize_sta = arg_pkt_size
	pktsize_min = 0
	pktsize_max = 0
	pktsize_inc = 0
	pktgen.pkt_size('all', 'start', pktsize_sta);
	pktgen.pkt_size('all', 'min'  , pktsize_min);
	pktgen.pkt_size('all', 'max'  , pktsize_max);
	pktgen.pkt_size('all', 'inc'  , pktsize_inc);
	printf('pkt_size %d [%d to %d] inc:%d\n',
		pktsize_sta, pktsize_min, pktsize_max, pktsize_inc)
	return
end


function get_avg(arry, size)
	local sum = 0;
	for i=1, size, 1 do
		sum = sum + arry[i]
	end
	return sum / size
end


function traffic_test(delay_msec, cnt)
	pktgen.clr();
	pktgen.start('all');
	local rx = {}
	local tx = {}
	pktgen.delay(2000)
	for i=1, cnt, 1 do
		local s = pktgen.portStats("all", "rate");
		local cur_rx = s[0]['mbits_rx'] + s[1]['mbits_rx']
		local cur_tx = s[0]['mbits_tx'] + s[1]['mbits_tx']

		table.insert(rx, cur_rx)
		table.insert(tx, cur_tx)
		printf("rx:%dMbps, tx:%dMbps \n", cur_rx, cur_tx)
		pktgen.delay(delay_msec)
	end
	pktgen.stop('all');
	return rx, tx
end


function main()
	cnt = 3
	setting(64)
	local rx64, tx64 = traffic_test(1000, cnt)
	setting(128)
	local rx128, tx128 = traffic_test(1000, cnt)
	print(64, get_avg(rx64, cnt), get_avg(tx64, cnt))
	print(128, get_avg(rx128, cnt), get_avg(tx128, cnt))
end


main()

