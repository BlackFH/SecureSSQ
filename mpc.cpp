#include "mpc.h"

void mpc_server::init()
{
	tcp.init();
}


int mpc_server::restore(int shared_x)
{
	int res, other;
	other = tcp.recv_int();
	tcp.send_int(shared_x);
	res = shared_x + other;
	return res;
}

int mpc_server::share(int x)
{
	int r = rand();
	int other = x - r;
	tcp.send_int(other);
	return r;
}


ArrayXi mpc_server::restore_v(ArrayXi shared_v)
{
	ArrayXi res, other;
	int len = shared_v.size();
	other = tcp.recv_arrayxi(len);
	tcp.send_arrayxi(shared_v);
	res = shared_v + other;
	return res;
}


ArrayXi mpc_server::share_v(ArrayXi v)
{
	int len = v.size();
	ArrayXi r = ArrayXi::Random(len);
	ArrayXi other = v - r;
	tcp.send_arrayxi(other);
	return r;
}



int mpc_server::cmp_simulate(int shared_a, int shared_b)
{
	int a = restore(shared_a);
	int b = restore(shared_b);
	int res;
	if (a > b) {
		res = 1;
	}
	else {
		res = 0;
	}
	//return res;
	int share_res = share(res);
	return share_res;
}




ArrayXi mpc_server::SSP_online(ArrayXi x, vector<int> pi, ArrayXi u)
{
	int len = x.size();
	ArrayXi L4 = x + u;
	tcp.send_arrayxi(L4);
	ArrayXi L5 = tcp.recv_arrayxi(len);
	ArrayXi s_L5 = shuffle(L5, pi);
	return s_L5;
}

void mpc_server::SHA(ArrayXi& x, int i, int s)
{
	int lchild = 2 * i + 1;
	int rchild = 2 * i + 2;
	int max = i;

	if (lchild < s) {
		int c1 = cmp_simulate(x[lchild], x[max]);
		int r_c1 = restore(c1);
		if (r_c1 == 1) {
			max = lchild;
		}
	}

	if (rchild < s) {
		int c2 = cmp_simulate(x[rchild], x[max]);
		int r_c2 = restore(c2);
		if (r_c2 == 1) {
			max = rchild;
		}
	}

	if (max != i) {
		int tmp = x[max];
		x[max] = x[i];
		x[i] = tmp;
		SHA(x, max, s);
	}

}

ArrayXi mpc_server::STKC(ArrayXi x, int k)
{
	int len = x.size();


	for (int i = len / 2 + 1; i >= 0; i--) {
		SHA(x, i, len);
		// x = x(seq(0, k));
	}
	for (int i = len - 1; i >= 1; i--) {
		//int c = cmp_simulate(x[1], ori[i]);
		//int r_c = restore(c);
		int tmp = x[0];
		x[0] = x[i];
		x[i] = tmp;
		SHA(x, 0, i);

	}
	ArrayXi topK(k);
	for (int i = 0; i < k; i++)
	{
		topK[i] = x[len - i - 1];
	}
	return topK;
}



void mpc_client::init()
{
	tcp.init();
}

int mpc_client::restore(int shared_x)
{
	int res, other;
	tcp.send_int(shared_x);
	other = tcp.recv_int();
	res = shared_x + other;
	return res;
}

int mpc_client::share()
{
	int x = tcp.recv_int();
	return x;
}


ArrayXi mpc_client::restore_v(ArrayXi shared_v)
{
	ArrayXi res, other;
	int len = shared_v.size();
	tcp.send_arrayxi(shared_v);
	other = tcp.recv_arrayxi(len);
	res = shared_v + other;
	return res;
}


ArrayXi mpc_client::share_v(int len)
{
	ArrayXi other = tcp.recv_arrayxi(len);
	return other;
}

int mpc_client::cmp_simulate(int shared_a, int shared_b)
{
	int a = restore(shared_a);
	int b = restore(shared_b);
	int share_res = tcp.recv_int();
	return share_res;
}

ArrayXi mpc_client::SSP_online(ArrayXi x, vector<int> pi, ArrayXi u)
{
	int len = x.size();
	ArrayXi L4 = tcp.recv_arrayxi(len);
	ArrayXi L5 = L4 + x + u;
	ArrayXi s_L5 = shuffle(L5, pi);
	tcp.send_arrayxi(s_L5);
	return s_L5;
}

void mpc_client::SHA(ArrayXi& x, int i, int s)
{
	int lchild = 2 * i + 1;
	int rchild = 2 * i + 2;
	int max = i;

	if (lchild < s) {
		int c1 = cmp_simulate(x[lchild], x[max]);
		int r_c1 = restore(c1);
		if (r_c1 == 1) {
			max = lchild;
		}
	}

	if (rchild < s) {
		int c2 = cmp_simulate(x[rchild], x[max]);
		int r_c2 = restore(c2);
		if (r_c2 == 1) {
			max = rchild;
		}
	}

	if (max != i) {
		int tmp = x[max];
		x[max] = x[i];
		x[i] = tmp;
		SHA(x, max, s);
	}
}

ArrayXi mpc_client::STKC(ArrayXi x, int k)
{
	int len = x.size();


	for (int i = len / 2 + 1; i >= 0; i--) {
		SHA(x, i, len);
		// x = x(seq(0, k));
	}
	for (int i = len - 1; i >= 1; i--) {
		//int c = cmp_simulate(x[1], ori[i]);
		//int r_c = restore(c);
		int tmp = x[0];
		x[0] = x[i];
		x[i] = tmp;
		SHA(x, 0, i);

	}
	ArrayXi topK(k);
	for (int i = 0; i < k; i++)
	{
		topK[i] = x[len - i - 1];
	}
	return topK;
}


ArrayXi shuffle(ArrayXi x, vector<int> idx)
{
	int len = x.size();
	ArrayXi x_shuffle(len);
	for (int i = 0; i < len; i++)
	{
		x_shuffle[i] = x[idx[i]];
	}
	return x_shuffle;
}


ArrayXi re_shuffle(ArrayXi x_shuffle, vector<int> idx)
{
	int len = x_shuffle.size();
	ArrayXi x(len);
	for (int i = 0; i < len; i++)
	{
		x[idx[i]] = x_shuffle[i];
	}
	return x;
}

vector<int> gen_pi(int len) {
	vector<int> pi(len);
	for (int i = 0; i < len; i++)
	{
		pi[i] = i;

	}
	random_shuffle(begin(pi), end(pi));
	return pi;
}

void print_array(vector<int> x) {
	int len = x.size();
	for (int i = 0; i < len; i++)
	{
		cout << x[i] << "\t";
	}
	cout << endl;
}

void print_array(ArrayXi x) {
	int len = x.size();
	for (int i = 0; i < len; i++)
	{
		cout << x[i] << "\t";
	}
	cout << endl;
}