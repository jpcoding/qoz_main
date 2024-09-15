//
// Created by Kai Zhao on 10/27/22.
//

#include "qozc.h"
#include "QoZ/api/sz.hpp"
#include <cstddef>
#include <type_traits>

// namespace SZ = QoZ;
using namespace QoZ;

unsigned char *compress_args(int dataType, void *data, size_t *outSize,
                             int errBoundMode, double absErrBound,
                             double relBoundRatio, double pwrBoundRatio,
                             size_t r5, size_t r4, size_t r3, size_t r2,
                             size_t r1) {

  QoZ::Config conf;
  if (r2 == 0) {
    conf = QoZ::Config(r1);
  } else if (r3 == 0) {
    conf = QoZ::Config(r2, r1);
  } else if (r4 == 0) {
    conf = QoZ::Config(r3, r2, r1);
  } else if (r5 == 0) {
    conf = QoZ::Config(r4, r3, r2, r1);
  } else {
    conf = QoZ::Config(r5 * r4, r3, r2, r1);
  }
  //    conf.loadcfg(conPath);
  conf.absErrorBound = absErrBound;
  conf.relErrorBound = relBoundRatio;
  //    conf.pwrErrorBound = pwrBoundRatio;
  if (errBoundMode == ABS) {
    conf.errorBoundMode = EB_ABS;
  } else if (errBoundMode == REL) {
    conf.errorBoundMode = EB_REL;
  } else if (errBoundMode == ABS_AND_REL) {
    conf.errorBoundMode = EB_ABS_AND_REL;
  } else if (errBoundMode == ABS_OR_REL) {
    conf.errorBoundMode = EB_ABS_OR_REL;
  } else {
    printf("errBoundMode %d not support\n ", errBoundMode);
    exit(0);
  }

  unsigned char *cmpr_data = NULL;
  if (dataType == SZ_FLOAT) {
    cmpr_data =
        (unsigned char *)SZ_compress<float>(conf, (float *)data, *outSize);
  } else if (dataType == SZ_DOUBLE) {
    // cmpr_data = (unsigned char *) SZ_compress<double>(conf, (double *) data,
    // *outSize);
  } else {
    printf("dataType %d not support\n", dataType);
    exit(0);
  }

  // convert c++ memory (by 'new' operator) to c memory (by malloc)
  auto *cmpr = (unsigned char *)malloc(*outSize);
  memcpy(cmpr, cmpr_data, *outSize);
  delete[] cmpr_data;

  return cmpr;
}

unsigned char *compress_conf(int dataType, void *data, size_t *outSize,
                             int errBoundMode, double absErrBound,
                             double relBoundRatio, double pwrBoundRatio,
                             size_t r5, size_t r4, size_t r3, size_t r2,
                             size_t r1, char *configFilePath) {

  QoZ::Config conf;

  printf("config file path: %s \n", configFilePath);
  std::cout << QoZ::ALGO_STR[conf.cmprAlgo] << std::endl;

  if (r2 == 0) {
    conf = QoZ::Config(r1);
  } else if (r3 == 0) {
    conf = QoZ::Config(r2, r1);
  } else if (r4 == 0) {
    conf = QoZ::Config(r3, r2, r1);
  } else if (r5 == 0) {
    conf = QoZ::Config(r4, r3, r2, r1);
  } else {
    conf = QoZ::Config(r5 * r4, r3, r2, r1);
  }
  conf.loadcfg(configFilePath);
  conf.absErrorBound = absErrBound;
  conf.relErrorBound = relBoundRatio;
  //    conf.pwrErrorBound = pwrBoundRatio;
  if (errBoundMode == ABS) {
    conf.errorBoundMode = EB_ABS;
  } else if (errBoundMode == REL) {
    conf.errorBoundMode = EB_REL;
  } else if (errBoundMode == ABS_AND_REL) {
    conf.errorBoundMode = EB_ABS_AND_REL;
  } else if (errBoundMode == ABS_OR_REL) {
    conf.errorBoundMode = EB_ABS_OR_REL;
  } else {
    printf("errBoundMode %d not support\n ", errBoundMode);
    exit(0);
  }

  unsigned char *cmpr_data = NULL;
  if (dataType == SZ_FLOAT) {
    std::cout << QoZ::ALGO_STR[conf.cmprAlgo] << std::endl;
    cmpr_data =
        (unsigned char *)SZ_compress<float>(conf, (float *)data, *outSize);
  } else if (dataType == SZ_DOUBLE) {
    // cmpr_data = (unsigned char *) SZ_compress<double>(conf, (double *) data,
    // *outSize);
  } else {
    printf("dataType %d not support\n", dataType);
    exit(0);
  }

  // convert c++ memory (by 'new' operator) to c memory (by malloc)
  auto *cmpr = (unsigned char *)malloc(*outSize);
  memcpy(cmpr, cmpr_data, *outSize);
  delete[] cmpr_data;

  return cmpr;
}

void *decompress(int dataType, unsigned char *bytes, size_t byteLength,
                 size_t r5, size_t r4, size_t r3, size_t r2, size_t r1) {
  size_t n = 0;
  if (r2 == 0) {
    n = r1;
  } else if (r3 == 0) {
    n = r1 * r2;
  } else if (r4 == 0) {
    n = r1 * r2 * r3;
  } else if (r5 == 0) {
    n = r1 * r2 * r3 * r4;
  } else {
    n = r1 * r2 * r3 * r4 * r5;
  }

  QoZ::Config conf;
  if (dataType == SZ_FLOAT) {
    auto dec_data = (float *)malloc(n * sizeof(float));
    SZ_decompress<float>(conf, (char *)bytes, byteLength, dec_data);
    return dec_data;
  }
  // else if (dataType == SZ_DOUBLE) {
  // auto dec_data = (double *) malloc(n * sizeof(double));
  // SZ_decompress<double>(conf, (char *) bytes, byteLength, dec_data);
  // return dec_data;
  // }
  else {
    printf("dataType %d not support\n", dataType);
    exit(0);
  }
}

void *decompress_conf(int dataType, unsigned char *bytes, size_t byteLength,
                      size_t r5, size_t r4, size_t r3, size_t r2, size_t r1,
                      char *configFilePath) {

  QoZ::Config conf;
  conf.loadcfg(configFilePath);
  printf("config file path: %s \n", configFilePath);
  size_t n = 0;
  if (r2 == 0) {
    n = r1;
  } else if (r3 == 0) {
    n = r1 * r2;
  } else if (r4 == 0) {
    n = r1 * r2 * r3;
  } else if (r5 == 0) {
    n = r1 * r2 * r3 * r4;
  } else {
    n = r1 * r2 * r3 * r4 * r5;
  }

  if (dataType == SZ_FLOAT) {
    auto dec_data = (float *)malloc(n * sizeof(float));
    SZ_decompress<float>(conf, (char *)bytes, byteLength, dec_data);
    return dec_data;
  }
  // else if (dataType == SZ_DOUBLE) {
  // auto dec_data = (double *) malloc(n * sizeof(double));
  // SZ_decompress<double>(conf, (char *) bytes, byteLength, dec_data);
  // return dec_data;
  // }
  else {
    printf("dataType %d not support\n", dataType);
    exit(0);
  }
}

void compress_decompress_conf_float_2d(float *data, float *ddata, size_t *dims,
                                       float eb, size_t &outsize,
                                       char *configFilePath) {
  size_t r1 = dims[0];
  size_t r2 = dims[1];
  size_t n = r1 * r2;
  outsize = n;
  QoZ::Config conf;
  conf = QoZ::Config(r2, r1);
  conf.loadcfg(configFilePath);
  conf.relErrorBound = eb;
  conf.errorBoundMode = QoZ::EB_REL;

  unsigned char *cmpr_data = NULL;
  cmpr_data = (unsigned char *)SZ_compress<float>(conf, data, outsize);
  std::cout << "outsize: " << outsize << std::endl; 

  QoZ::Config conf2;
  auto dec_data = (float *)malloc(n * sizeof(float));
  SZ_decompress<float>(conf2, (char *)cmpr_data, outsize, dec_data);
  std::copy(dec_data, dec_data + n, ddata);
  delete[]cmpr_data;
  free(dec_data);
}

void compress_decompress_conf_float_3d(float *data, float *ddata, size_t *dims,
                                       float eb, size_t &outsize,
                                       char *configFilePath) {
  size_t r1 = dims[0];
  size_t r2 = dims[1];
  size_t r3 = dims[2];
  size_t n = r1 * r2 * r3;
  outsize = n;
  QoZ::Config conf;
  conf = QoZ::Config(r3, r2, r1); 

  conf.loadcfg(configFilePath);
  conf.errorBoundMode = QoZ::EB_REL;
  conf.relErrorBound = eb;
  unsigned char *cmpr_data = NULL;
  cmpr_data = (unsigned char *)SZ_compress<float>(conf, data, outsize);
  auto dec_data = (float *)malloc(n * sizeof(float));
  SZ_decompress<float>(conf, (char *)cmpr_data, outsize, dec_data);
  std::copy(dec_data, dec_data + n, ddata);
  free(dec_data);
  delete[] cmpr_data;
}

void compress_decompress_conf_float(float *data, float *ddata, int N,
                                               size_t *dims, float eb,
                                               size_t &outsize,
                                               char *configFilePath) {
  if (N == 2) {
    compress_decompress_conf_float_2d(data, ddata, dims, eb, outsize,
                                      configFilePath);
  } else if (N == 3) {
    compress_decompress_conf_float_3d(data, ddata, dims, eb, outsize,
                                      configFilePath);
  } else {
    printf("N=%d not support\n", N);
    exit(0);
  }
}