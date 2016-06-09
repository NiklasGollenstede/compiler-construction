bool greater_than(int lhs, int rhs) {
	return lhs > rhs;
}

string to_string(int i) {
	return "NUMBER";
}

void test_operand_types() {

	bool b0, b1;
	b0 = true;
	b1 = false;
	b0 && b1;
	b0 || b1;
	
	int i0, i1;
	i0 = 0;
	i1 = 1;
	i0 + i1;
	i0 - i1;
	i0 * i1;
	i0 / i1;
	i0++;
	i0--;
	++i0;
	--i0;
	i0 > i1;
	i0 >= i1;
	i0 <= i1;
	i0 < i1;
		
	double d0, d1;
	d0 = 0.0;
	d1 = 1.0;
	d0 + d1;
	d0 - d1;
	d0 * d1;
	d0 / d1;
	d0 > d1;
	d0 >= d1;
	d0 <= d1;
	d0 < d1;
	
	string s0, s1;
	s0 = "hello";
	s1 = "world";
	s0 + s1 + to_string(i0);
}
