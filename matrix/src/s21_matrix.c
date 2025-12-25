#include "s21_matrix.h"

// 1 — OK;
// 0 — Invalid matrix structure (Ошибка, некорректная матрица)
// 2 — Mathematical calculation error (Ошибка вычисления)
// eq:
// #define SUCCESS 0
// #define FAILURE 1

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = OK;
  if ((rows <= 0) || (columns <= 0) || result == NULL) {
    res = INCORRECT_MATRIX;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = malloc(rows * sizeof(double *));

    for (int i = 0; i < rows; i++) {
      result->matrix[i] = malloc(columns * sizeof(double));
    };
  }
  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < (A->rows); i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = OK;
  int res_eq = 3;
  if ((s21_check_matrix(A) == INCORRECT_MATRIX) ||
      (s21_check_matrix(B) == INCORRECT_MATRIX)) {
    res = INCORRECT_MATRIX;
  }
  if (res == OK) {
    if ((A->columns != B->columns) || (A->rows != B->rows)) res_eq = FAILURE;
  }

  if (res == OK && res_eq != FAILURE) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6) {
          res_eq = FAILURE;
        }
      }
    }
  }
  if (res_eq == 3) {
    if (res == OK)
      res_eq = SUCCESS;
    else
      res_eq = INCORRECT_MATRIX;
  }
  return res_eq;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if ((s21_check_matrix(A) == INCORRECT_MATRIX) ||
      (s21_check_matrix(B) == INCORRECT_MATRIX) || result == NULL) {
    res = INCORRECT_MATRIX;
  }
  if (res == OK) res = s21_check_matrix_size(A, B);
  if (res == OK) res = s21_create_matrix(A->rows, A->columns, result);

  if (res == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if ((s21_check_matrix(A) == INCORRECT_MATRIX) ||
      (s21_check_matrix(B) == INCORRECT_MATRIX) || result == NULL)
    res = INCORRECT_MATRIX;
  if (res == OK) res = (s21_check_matrix_size(A, B));
  if (res == OK) res = s21_create_matrix(A->rows, A->columns, result);
  if (res == OK) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = OK;
  if (s21_check_matrix(A) == INCORRECT_MATRIX || result == NULL) {
    res = INCORRECT_MATRIX;
  } else {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = number * A->matrix[i][j];
        }
      }
    }
  }
  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (s21_check_matrix(A) == INCORRECT_MATRIX ||
      s21_check_matrix(B) == INCORRECT_MATRIX) {
    res = INCORRECT_MATRIX;
  } else if (A->columns != B->rows) {
    res = CALC_ERROR;
  }
  if (res == OK) {
    res = s21_create_matrix(A->rows, B->columns, result);
    if (res == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          result->matrix[i][j] = 0;
          for (int k = 0; k < A->columns; k++) {
            result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
          }
        }
      }
    }
  }
  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = s21_check_matrix(A);
  if (res == OK) {
    res = s21_create_matrix(A->columns, A->rows, result);
    if (res == OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  }
  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = OK;
  if (s21_check_matrix(A) == INCORRECT_MATRIX || result == NULL)
    res = INCORRECT_MATRIX;
  else if (A->columns != A->rows)
    res = CALC_ERROR;

  if (res == OK) {
    s21_calculate_determinant(A, result);
  }
  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int sign = 1;
  matrix_t minor_matrix = {0};
  int res = s21_check_matrix(A);
  if (result == NULL) res = INCORRECT_MATRIX;

  if (res == OK) {
    if (A->rows != A->columns) res = CALC_ERROR;
  }
  if (res == OK) {
    if (A->rows == 1) {
      res = s21_create_matrix(1, 1, result);
      if (res == OK) {
        result->matrix[0][0] = 1.0;
      }
    } else {
      if (res == OK)
        res = s21_create_matrix(A->rows - 1, A->columns - 1, &minor_matrix);
      if (res == OK) res = s21_create_matrix(A->rows, A->columns, result);

      if (res == OK) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            s21_get_minor_matrix(A, &minor_matrix, i, j);
            res = s21_determinant(&minor_matrix, &result->matrix[i][j]);
            if (res == OK) result->matrix[i][j] = result->matrix[i][j] * sign;
          }
        }
      }
      s21_remove_matrix(&minor_matrix);
    }
  }
  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = OK;
  matrix_t B = {0}, C = {0};

  res = s21_check_matrix(A);
  if (res == OK && A->columns != A->rows) {
    res = CALC_ERROR;
  }

  if (res == OK) {
    double det = 0.0;
    res = s21_determinant(A, &det);
    if (res == OK && fabs(det) < 1e-7) {
      res = CALC_ERROR;
    }

    if (res == OK) {
      res = s21_calc_complements(A, &B);
    }

    if (res == OK) {
      res = s21_transpose(&B, &C);
    }

    if (res == OK) {
      res = s21_mult_number(&C, 1.0 / det, result);
    }
  }

  s21_remove_matrix(&B);
  s21_remove_matrix(&C);

  return res;
}

///////

int s21_check_matrix(matrix_t *A) {
  int res = OK;
  if ((A == NULL) || (A->columns <= 0) || (A->rows <= 0) ||
      (A->matrix == NULL)) {
    res = INCORRECT_MATRIX;
  }
  return res;
}

int s21_check_matrix_size(matrix_t *A, matrix_t *B) {
  int res = OK;
  if (A->columns != B->columns || A->rows != B->rows) {
    res = CALC_ERROR;
  }
  return res;
}

void s21_get_minor_matrix(matrix_t *A, matrix_t *minor_matrix, int i_minor,
                          int j_minor) {
  int minor_row = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i != i_minor) {
      int minor_col = 0;
      for (int j = 0; j < A->columns; j++) {
        if (j != j_minor) {
          minor_matrix->matrix[minor_row][minor_col] = A->matrix[i][j];
          minor_col++;
        }
      }
      minor_row++;
    }
  }
}

void s21_calculate_determinant(matrix_t *A, double *det) {
  if (A->rows == 1) {
    *det = A->matrix[0][0];
  } else if (A->rows == 2) {
    *det =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    int sign = 1;
    double minor_det = 0;
    *det = 0.0;

    for (int i = 0; i < A->rows; i++) {
      sign = (i % 2 == 0) ? 1 : -1;
      matrix_t minor;
      int res = s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
      if (res == OK) {
        s21_get_minor_matrix(A, &minor, 0, i);
        minor_det = 0;
        s21_calculate_determinant(&minor, &minor_det);
        *det += sign * A->matrix[0][i] * minor_det;
        s21_remove_matrix(&minor);
      }
    }
  }
}
