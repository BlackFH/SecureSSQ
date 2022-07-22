#pragma once

#include "tcp.h"

class mpc_server
{
public:
	tcp_server tcp;

	void init();
	int restore(int shared_x);
	int share(int x);
	ArrayXi restore_v(ArrayXi shared_x);
	ArrayXi share_v(ArrayXi x);
	int cmp_simulate(int shared_a, int shared_b);
	ArrayXi SSP_online(ArrayXi x, vector<int> pi, ArrayXi u);
	void SHA(ArrayXi& x, int i, int s);
	ArrayXi STKC(ArrayXi x, int k);
	void SAGSC();
	void SQ();
};


class mpc_client
{
public:
	tcp_client tcp;

	void init();
	int share();
	int restore(int shared_x);
	ArrayXi restore_v(ArrayXi shared_x);
	ArrayXi share_v(int len);
	int cmp_simulate(int shared_a, int shared_b);

	ArrayXi SSP_online(ArrayXi x, vector<int> pi, ArrayXi u);
	void SHA(ArrayXi& x, int i, int s);
	ArrayXi STKC(ArrayXi x, int k);
};

ArrayXi shuffle(ArrayXi x, vector<int> idx);
ArrayXi re_shuffle(ArrayXi x_shuffle, vector<int> idx);
vector<int> gen_pi(int len);
void print_array(vector<int> x);
void print_array(ArrayXi x);