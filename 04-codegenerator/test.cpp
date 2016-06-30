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

bool calcInt() {
	int i;
	int j = 42;
	i = 2;
	j = i + j;
	i = j * i;
	j = i / j;
	i = j - i;
	return i == j && i != j || i < j && j <= i || j > i && j >= i;
}

bool calcDouble() {
	double i;
	double j = 42.0;
	i = 2.0;
	j = i + j;
	i = j * i;
	j = i / j;
	i = j - i;
	return i == j && i != j || i < j && j <= i || j > i && j >= i;
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

int manyArgs(int a, int b, int c, int d, int e) {
	return 0;
}

void callMany() {
	manyArgs(1, 2, 3, 4, 5);
}

int checkDouble() {
	double d = 1.0;
	double d2 = 2.5;
	if (d - d2 / 2.0 > 0.0)
		return 0;
	else
		return 1;
}

int main() {
	callMany();
	return checkDouble();
}
