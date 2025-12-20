#include "s21_tests.h"

START_TEST(create_matrix_1_1) {
  matrix_t A;
  int result = s21_create_matrix(1, 1, &A);
  ck_assert_int_eq(A.rows, 1);
  ck_assert_int_eq(A.columns, 1);
  ck_assert_int_eq(result, 0);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_matrix_3_3) {
  matrix_t A;
  int result = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 3);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_invalid_matrix) {
  matrix_t A;
  int result = s21_create_matrix(-1, 0, &A);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(eq_matrix_equal_2x2) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int res = s21_eq_matrix(&A, &B);

  ck_assert_int_eq(res, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_not_equal_2x2) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.1;

  int res = s21_eq_matrix(&A, &B);

  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_diff_sizes) {
  matrix_t A, B;
  s21_create_matrix(2, 4, &A);
  s21_create_matrix(3, 5, &B);

  int res = s21_eq_matrix(&A, &B);

  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_invalid_A) {
  matrix_t A = {0};
  matrix_t B;
  s21_create_matrix(2, 2, &B);

  int res = s21_eq_matrix(&A, &B);

  ck_assert_int_eq(res, INCORRECT_MATRIX);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_2x2) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  B.matrix[0][0] = 5;
  B.matrix[0][1] = 6;
  B.matrix[1][0] = 7;
  B.matrix[1][1] = 8;

  // A + B = [[6, 8],
  //          [10, 12]]

  int res = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], 6.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], 8.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], 10.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 12.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sum_matrix_3x3) {
  matrix_t A, B, result;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = (i * 3 + j + 1) * 10;
      B.matrix[i][j] = i * 3 + j + 1;
    }
  }

  // A + B = [[11, 22, 33],
  //          [44, 55, 66],
  //          [77, 88, 99]]

  int res = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double expected = A.matrix[i][j] + B.matrix[i][j];
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sum_matrix_err_size) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);

  int res = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_err_invalid_A) {
  matrix_t A = {0};
  matrix_t B, result;
  s21_create_matrix(2, 2, &B);

  int res = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(res, INCORRECT_MATRIX);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mul_matrix_1) {
  matrix_t A;
  matrix_t B;
  matrix_t result;
  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 5, &B);
  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mul_matrix_2) {
  matrix_t A;
  matrix_t B;
  matrix_t result;
  int create_A = s21_create_matrix(3, 2, &A);
  int create_B = s21_create_matrix(2, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[1][0] = 2;
  A.matrix[2][0] = 3;
  A.matrix[0][1] = 4;
  A.matrix[1][1] = 5;
  A.matrix[2][1] = 6;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 4;
  B.matrix[1][1] = 5;
  B.matrix[1][2] = 6;

  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);
  ck_assert_int_eq(result.matrix[0][0], 17);
  ck_assert_int_eq(result.matrix[1][1], 29);
  ck_assert_int_eq(result.matrix[2][2], 45);
  if (create_A == OK) s21_remove_matrix(&A);
  if (create_B == OK) s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mul_matrix_err_1) {
  matrix_t A;
  matrix_t B;
  matrix_t result;

  int create_A = s21_create_matrix(3, -1, &A);
  int create_B = s21_create_matrix(1, 2, &B);
  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 1);

  if (create_A == OK) s21_remove_matrix(&A);
  if (create_B == OK) s21_remove_matrix(&B);
}
END_TEST

START_TEST(mul_matrix_err_2) {
  matrix_t A;
  matrix_t B;
  matrix_t result;
  int create_A = s21_create_matrix(3, 3, &A);
  int create_B = s21_create_matrix(2, 2, &B);
  int res = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(res, 2);

  if (create_A == OK) s21_remove_matrix(&A);
  if (create_B == OK) s21_remove_matrix(&B);
}
END_TEST

START_TEST(transpose_matrix) {
  matrix_t A;
  matrix_t result;

  int create_A = s21_create_matrix(3, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[1][0] = 2;
  A.matrix[2][0] = 3;
  A.matrix[0][1] = 4;
  A.matrix[1][1] = 5;
  A.matrix[2][1] = 6;
  int res = s21_transpose(&A, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.rows, A.columns);
  ck_assert_int_eq(result.matrix[0][1], A.matrix[1][0]);

  if (create_A == OK) {
    s21_remove_matrix(&A);
    s21_remove_matrix(&result);
  }
}
END_TEST

START_TEST(transpose_matrix_err) {
  matrix_t A;
  matrix_t result;

  int res = s21_create_matrix(3, -1, &A);
  if (res == OK) {
    res = s21_transpose(&A, &result);
  }
  ck_assert_int_eq(res, 1);

  if (res == OK) s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_1x1) {
  matrix_t A;
  double det = 0.0;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int res = s21_determinant(&A, &det);

  ck_assert_int_eq(res, OK);
  ck_assert_ldouble_eq(det, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_2x2) {
  matrix_t A;
  double det = 0.0;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 3.0;
  A.matrix[1][0] = 1.0;
  A.matrix[1][1] = 4.0;

  // det = 5
  int res = s21_determinant(&A, &det);

  ck_assert_int_eq(res, OK);
  ck_assert_ldouble_eq(det, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_3x3) {
  matrix_t A;
  double det = 0.0;
  s21_create_matrix(3, 3, &A);
  // 1  2  3
  // 4  5  6
  // 7  8  9, det = 0
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;

  int res = s21_determinant(&A, &det);

  ck_assert_int_eq(res, OK);
  ck_assert_ldouble_eq(det, 0.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_3x3_nonzero) {
  matrix_t A;
  double det = 0.0;
  s21_create_matrix(3, 3, &A);
  // 2  -3  1
  // 2   0 -1
  // 1   4  5
  // det = 49
  A.matrix[0][0] = 2;
  A.matrix[0][1] = -3;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 0;
  A.matrix[1][2] = -1;
  A.matrix[2][0] = 1;
  A.matrix[2][1] = 4;
  A.matrix[2][2] = 5;

  int res = s21_determinant(&A, &det);

  ck_assert_int_eq(res, OK);
  ck_assert_ldouble_eq_tol(det, 49.0, 1e-6);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_4x4) {
  matrix_t A;
  double det = 0.0;
  s21_create_matrix(4, 4, &A);
  // Е, det = 1
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      A.matrix[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }

  int res = s21_determinant(&A, &det);

  ck_assert_int_eq(res, OK);
  ck_assert_ldouble_eq(det, 1.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_not_square) {
  matrix_t A;
  double det = 0.0;
  s21_create_matrix(2, 3, &A);

  int res = s21_determinant(&A, &det);

  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_1x1) {
  matrix_t A, result;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int res = s21_calc_complements(&A, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 1);
  ck_assert_int_eq(result.columns, 1);
  ck_assert_ldouble_eq(result.matrix[0][0], 1.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_2x2) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  // [[1, 2],
  //  [3, 4]]
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  // result = [[4, -3],
  //           [-2, 1]]

  int res = s21_calc_complements(&A, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_ldouble_eq_tol(result.matrix[0][0], 4.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], -3.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], -2.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 1.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_3x3_identity) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);
  // Е
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }

  int res = s21_calc_complements(&A, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double expected = (i == j) ? 1.0 : 0.0;
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_not_square) {
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);

  int res = s21_calc_complements(&A, &result);

  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_2x2) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  // A = [[4, 7],
  //      [2, 6]]
  // det = 10
  // 1/A = (1/10) * [[6, -7], [-2, 4]] = [[0.6, -0.7], [-0.2, 0.4]]
  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  int res = s21_inverse_matrix(&A, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);
  ck_assert_ldouble_eq_tol(result.matrix[0][0], 0.6, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], -0.7, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], -0.2, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 0.4, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_identity_3x3) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);
  // Е
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }

  int res = s21_inverse_matrix(&A, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double expected = (i == j) ? 1.0 : 0.0;
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_singular_matrix) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  // A = [[1, 2],
  //      [2, 4]], det = 0
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  int res = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(res, CALC_ERROR);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_not_square) {
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);

  int res = s21_inverse_matrix(&A, &result);

  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_null_A) {
  matrix_t result;
  int res = s21_inverse_matrix(NULL, &result);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
}
END_TEST

START_TEST(inverse_null_result) {
  matrix_t A;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 2;
  A.matrix[0][1] = 1;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;

  int res = s21_inverse_matrix(&A, NULL);
  ck_assert_int_eq(res, INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(sub_matrix_2x2) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 5;
  A.matrix[0][1] = 3;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[1][0] = 1;
  B.matrix[1][1] = 3;

  // A - B = [[4, 1],
  //          [1, 1]]

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 2);
  ck_assert_int_eq(result.columns, 2);

  ck_assert_ldouble_eq_tol(result.matrix[0][0], 4.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[0][1], 1.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][0], 1.0, 1e-6);
  ck_assert_ldouble_eq_tol(result.matrix[1][1], 1.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_3x3) {
  matrix_t A, B, result;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  // A = [[10, 20, 30],
  //      [40, 50, 60],
  //      [70, 80, 90]]
  // B = [[1,  2,  3 ],
  //      [4,  5,  6 ],
  //      [7,  8,  9 ]]
  // A - B = [[9, 18, 27],
  //          [36, 45, 54],
  //          [63, 72, 81]]

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = (i * 3 + j + 1) * 10;
      B.matrix[i][j] = i * 3 + j + 1;
    }
  }

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, OK);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double expected = A.matrix[i][j] - B.matrix[i][j];
      ck_assert_ldouble_eq_tol(result.matrix[i][j], expected, 1e-6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_err_size) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);  // 2x3
  s21_create_matrix(3, 2, &B);  // 3x2

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, CALC_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_err_invalid_A) {
  matrix_t A = {0};  // rows=0, columns=0, matrix=NULL
  matrix_t B, result;
  s21_create_matrix(2, 2, &B);

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, INCORRECT_MATRIX);

  s21_remove_matrix(&B);
}
END_TEST

Suite *s21_matrix_tests(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, create_matrix_1_1);
  tcase_add_test(tc, create_matrix_3_3);
  tcase_add_test(tc, create_invalid_matrix);
  tcase_add_test(tc, eq_matrix_equal_2x2);
  tcase_add_test(tc, eq_matrix_not_equal_2x2);
  tcase_add_test(tc, eq_matrix_diff_sizes);
  tcase_add_test(tc, eq_matrix_invalid_A);
  tcase_add_test(tc, sum_matrix_2x2);
  tcase_add_test(tc, sum_matrix_3x3);
  tcase_add_test(tc, sum_matrix_err_size);
  tcase_add_test(tc, sum_matrix_err_invalid_A);
  tcase_add_test(tc, mul_matrix_1);
  tcase_add_test(tc, mul_matrix_2);
  tcase_add_test(tc, mul_matrix_err_1);
  tcase_add_test(tc, mul_matrix_err_2);
  tcase_add_test(tc, transpose_matrix);
  tcase_add_test(tc, transpose_matrix_err);
  tcase_add_test(tc, determinant_1x1);
  tcase_add_test(tc, determinant_2x2);
  tcase_add_test(tc, determinant_3x3);
  tcase_add_test(tc, determinant_3x3_nonzero);
  tcase_add_test(tc, determinant_4x4);
  tcase_add_test(tc, determinant_not_square);
  tcase_add_test(tc, calc_complements_1x1);
  tcase_add_test(tc, calc_complements_2x2);
  tcase_add_test(tc, calc_complements_3x3_identity);
  tcase_add_test(tc, calc_complements_not_square);
  tcase_add_test(tc, inverse_2x2);
  tcase_add_test(tc, inverse_identity_3x3);
  tcase_add_test(tc, inverse_singular_matrix);
  tcase_add_test(tc, inverse_not_square);
  tcase_add_test(tc, inverse_null_A);
  tcase_add_test(tc, inverse_null_result);
  tcase_add_test(tc, sub_matrix_2x2);
  tcase_add_test(tc, sub_matrix_3x3);
  tcase_add_test(tc, sub_matrix_err_size);
  tcase_add_test(tc, sub_matrix_err_invalid_A);

  suite_add_tcase(suite, tc);

  return suite;
}

int main(void) {
  int failed = 0;
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, s21_matrix_tests());
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);

  printf("========= Failed %d =========\n", failed);
  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}
