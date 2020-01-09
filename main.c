#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

typedef struct matrix {
	char name; //�̸�
	int** elements; //��� �迭
	int row; //��
	int column; //��
}matrix;

FILE* file_pointer; //�Է� ���� ������
matrix Matrix[3]; //���Ͽ��� �ҷ��� ��� ����ü
char str[60]; //���� ���ڿ�
char** divide_str; //'+', '-'�� ���� ���� ���ڿ��� ���� �����ϴ� ������ ������ �迭
int* symbol; //��ȣ ���� ������ �迭
int cnt; //'+', '-' ������ ��Ÿ���� ����
int flag; //���� ���� �Ұ��� ���� �Ǵ� ����

void printMatrix(matrix M); //��� ��� �Լ�

void fetchMatrix(); //���� ��ġ �Լ�
void searchFile(); //�Է��� ������ �ִ��� ���θ� Ȯ��, �����ϴ� �Լ�
void divideByPlus(); //'+', '-'�� ���� ���� ��Ʈ���� ���� �������ִ� �Լ�

matrix scalarMultiplication(matrix M, int scalar); //��Į�� �� �Լ�
matrix matrixMultiplication(matrix M1, matrix M2); //��� �� �� �Լ�
matrix multiplicateDivideStr(char* string, int s); //��Į��, ��� �� �� ���� �Լ�
matrix matrixAddition(matrix M1, matrix M2); //��� �� �� �Լ�

int main() {
	int i;
	matrix* multiple_M = (matrix*)malloc(sizeof(matrix) * 3);

	flag = 0;
	symbol = (int*)malloc(sizeof(int) * 3);
	divide_str = (char**)malloc(sizeof(char*) * 3);
	for (i = 0; i <= cnt; i++) {
		divide_str[i] = (char*)malloc(sizeof(char) * 20);
	}

	searchFile(); //�Է��� ���� �˻� �� ��� ������ ��ġ

	while (1) {
		printf("������ �Է��ϼ��� (exit�� �Է��ϸ� ����˴ϴ�.): ");
		scanf("%s", str);

		if (strcmp(str, "exit") == 0 || strcmp(str, "EXIT") == 0) {
			printf("���α׷��� �����մϴ�.\n");
			break;
		}
		divideByPlus();

		multiple_M = (matrix*)realloc(multiple_M, sizeof(matrix) * (cnt + 1));
		for (i = 0; i <= cnt; i++) {
			multiple_M[i] = multiplicateDivideStr(divide_str[i], symbol[i]);
		}

		for (i = 0; i < cnt; i++) {
			multiple_M[i + 1] = matrixAddition(multiple_M[i], multiple_M[i + 1]);
		}

		if (flag == 0) {
			printMatrix(multiple_M[cnt]);

		}
		else {
			printf("��� �� ������ �Ұ����մϴ�.\n");
		}
		flag = 0;
		str[0] = '\0';
		
	}

	return 0;
}

void printMatrix(matrix M) {
	int i, j;

	for (i = 0; i < M.row; i++) {
		for (j = 0; j < M.column; j++) {
			printf("%d ", M.elements[i][j]);
		}
		printf("\n");
	}

	return;
}

void fetchMatrix() {
	int i, j, k;
	int tmp;
	char tmp2;

	for (i = 0; i < 3; i++) {
		fscanf(file_pointer, "%c", &Matrix[i].name);
		fscanf(file_pointer, "%c", &tmp2);
		fscanf(file_pointer, "%d", &Matrix[i].row);
		fscanf(file_pointer, "%c", &tmp2);
		fscanf(file_pointer, "%d", &Matrix[i].column);

		Matrix[i].elements = (int**)malloc(sizeof(int*) * Matrix[i].row);
		for (j = 0; j < Matrix[i].row; j++) {
			Matrix[i].elements[j] = (int*)malloc(sizeof(int) * Matrix[i].column);
		}

		for (j = 0; j < Matrix[i].row; j++) {
			for (k = 0; k < Matrix[i].column; k++) {
				fscanf(file_pointer, "%d", &tmp);

				Matrix[i].elements[j][k] = tmp;

				fscanf(file_pointer, "%c", &tmp2);
			}
		}

		printf("��� %c = \n", Matrix[i].name);
		printMatrix(Matrix[i]);
	}

	return;
}

void searchFile() {
	char file_path[100] = "\0";
	char file_root[10] = "\0";
	char file_name[20] = "\0";
	int result;

	strcat(file_root, "./");

	while (1) {
		printf("Ȯ���ڸ�(.txt)�� ������ ���� �̸��� �Է��ϼ���: ");
		scanf("%s", file_name);

		strcat(file_name, ".txt");
		strcat(file_path, file_root);
		strcat(file_path, file_name);

		result = access(file_path, 0);

		if (result == 0) {
			printf("�ش� ������ �����ϹǷ� ���� ����� �ҷ��ɴϴ�.\n");
			break;
		}
		else {
			printf("�ش� ������ �������� �ʽ��ϴ�.\n");
			file_path[0] = '\0';
		}
	}

	file_pointer = fopen(file_path, "r");
	fetchMatrix();
	fclose(file_pointer);

	return;
}

void divideByPlus() {
	int i = 0, j = 0, k = 0;

	cnt = 0;
	for (i = 0; i < strlen(str); i++) {
		if (str[i] == '+' || str[i] == '-') {
			cnt++;
		}
	}

	if (str[0] == '-') {
		cnt--;
	}

	symbol = (int*)realloc(symbol, sizeof(int) * (cnt + 1));

	divide_str = (char**)realloc(divide_str, sizeof(char*) * (cnt + 1));
	for (i = 0; i <= cnt; i++) {
		divide_str[i] = (char*)malloc(sizeof(char) * 20);
	}

	if (str[0] == '-') {
		symbol[0] = -1;
		i = 1;
	}
	else {
		symbol[0] = 1;
		i = 0;
	}

	j = 0;
	k = 0;
	for (; i < strlen(str); i++) {

		if (str[i] == '+') {
			divide_str[j][k] = '\0';
			j++;
			symbol[j] = 1;
			k = 0;
		}
		else if (str[i] == '-') {
			divide_str[j][k] = '\0';
			j++;
			symbol[j] = -1;
			k = 0;
		}
		else {
			divide_str[j][k] = str[i];
			k++;
		}
	}
	divide_str[j][k] = '\0';

	return;
}

matrix scalarMultiplication(matrix M, int scalar) {
	int i, j;
	matrix result;

	result.row = M.row;
	result.column = M.column;
	result.elements = (int**)malloc(sizeof(int*) * result.row);
	for (i = 0; i < result.row; i++) {
		result.elements[i] = (int*)malloc(sizeof(int) * result.column);
	}

	for (i = 0; i < result.row; i++) {
		for (j = 0; j < result.column; j++) {
			result.elements[i][j] = scalar * M.elements[i][j];
		}
	}

	return result;
}

matrix matrixMultiplication(matrix M1, matrix M2) {
	matrix result;
	int i, j, k;
	int sum = 0;

	result.elements = (int**)malloc(sizeof(int*) * M1.row);
	for (i = 0; i < M1.row; i++) {
		result.elements[i] = (int*)malloc(sizeof(int) * M2.column);
	}

	result.row = M1.row;
	result.column = M2.column;

	for (i = 0; i < M1.row; i++) {
		for (j = 0; j < M2.column; j++) {
			for (k = 0; k < M1.column; k++) {
				sum += M1.elements[i][k] * M2.elements[k][j];
			}
			result.elements[i][j] = sum;
			sum = 0;
		}
	}

	return result;
}

matrix multiplicateDivideStr(char* string, int s) {
	int i, j;
	int scalar = atoi(string);
	matrix tmp;

	tmp.row = 0;
	tmp.column = 0;

	i = 0;
	while (string[i] != Matrix[0].name && string[i] != Matrix[1].name && string[i] != Matrix[2].name) {
		i++;
	}

	for (j = 0; j < 3; j++) {
		if (string[i] == Matrix[j].name) {
			if (scalar == 0) {
				tmp = scalarMultiplication(Matrix[j], s);
			}
			else {
				tmp = scalarMultiplication(Matrix[j], scalar * s);
			}
		}
	}
	i++;

	while (string[i]) {
		for (j = 0; j < 3; j++) {
			if (string[i] == Matrix[j].name) {
				if (tmp.column != Matrix[j].row) {
					flag = 1;
					return tmp;
				}
				tmp = matrixMultiplication(tmp, Matrix[j]);
			}
		}
		i++;
	}

	return tmp;
}

matrix matrixAddition(matrix M1, matrix M2) {
	int i, j;
	matrix result;

	result.row = M1.row;
	result.column = M2.column;
	result.elements = (int**)malloc(sizeof(int*) * result.row);
	for (i = 0; i < result.row; i++) {
		result.elements[i] = (int*)malloc(sizeof(int) * result.column);
	}

	if (M1.row == M2.row && M1.column == M2.column) {
		for (i = 0; i < M1.row; i++) {
			for (j = 0; j < M2.column; j++) {
				result.elements[i][j] = M2.elements[i][j] + M1.elements[i][j];
			}
		}
	}
	else {
		flag = 1;
		return result;
	}

	return result;
}