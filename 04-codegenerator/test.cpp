double sin(double x) {
	return //1.0;
		0.0
		- x / 1.0
		+ x * x / 2.0
		- x * x * x / 6.0
		+ x * x * x * x / 24.0;
}

double ifElse() {
	double z;
	double x = sin(1.0);

	if(x > 0.0) {
		double y;
		x = x + 5.0;
		x = x - 2.0;
		y = 2.0 / x;
	} else {
		x = 3.0;
		double m;
	}

	return x;
}

int loop() {
	int i = 0;

	while (
		i < 100
	) {
		i = i + i;
	}

	return i;
}

bool boolFunc() {
	bool b = true;
	return b;
}

int manyArgs(int a, int b, int c, int d, int e) {
	return 0;
}

void callMany() {
	manyArgs(1, 2, 3, 4, 5);
}
