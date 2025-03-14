/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. The ASF
 * licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef _ST_HPC_PPL_CV_OCL_CALCHIST_H_
#define _ST_HPC_PPL_CV_OCL_CALCHIST_H_

#include "CL/cl.h"

#include "ppl/common/retcode.h"
#include "ppl/cv/ocl/kerneltypes.h"

namespace ppl {
namespace cv {
namespace ocl {

/**
 * @brief Equalizes the histogram of a grayscale image.
 * @param queue           opencl command queue.
 * @param height          input&output image's height.
 * @param width           input&output image's width.
 * @param inWidthStride   input image's width stride, which is not less than
 *                        `width * channels`.
 * @param inData          input image data, it should be a buffer object.
 * @param outWidthStride  the width stride of output image, similar to
 *                        inWidthStride.
 * @param outData         output image data, it should be a buffer object.
 * @return The execution status, succeeds or fails with an error code.
 * @note 1 For best performance, this function needs a memory pool to store the intermediate
 *         histogram, which is not less than 1024 bytes.
 *       2 The size of the output histogram is 256.
 * @warning All input parameters must be valid, or undefined behaviour may occur.
 * @remark The fllowing table show which data type and channels are supported.
 * <table>
 * <tr><th>Data type(T)<th>channels
 * <tr><td>uint8_t(uchar)<td>1
 * </table>
 * <table>
 * <caption align="left">Requirements</caption>
 * <tr><td>OpenCL platforms supported <td>OpenCL 1.2
 * <tr><td>Header files  <td> #include "ppl/cv/ocl/equalizehist.h"
 * <tr><td>Project       <td> ppl.cv
 * </table>
 * @since ppl.cv-v1.0.0
 * ###Example
 * @code{.cpp}
 * #include "ppl/cv/ocl/equalizehist.h"
 * #include "ppl/common/oclcommon.h"
 * #include "ppl/cv/ocl/use_memory_pool.h"
 * using namespace ppl::common::ocl;
 * using namespace ppl::cv::ocl;
 *
 * int main(int argc, char** argv) {
 *   int width  = 640;
 *   int height = 480;
 *   int channels = 1;
 *
 *   createSharedFrameChain(false);
 *   cl_context context = getSharedFrameChain()->getContext();
 *   cl_command_queue queue = getSharedFrameChain()->getQueue();
 *   activateGpuMemoryPool(1024);
 *
 *   cl_int error_code = 0;
 *   int data_size = height * width * channels * sizeof(uchar);
 *   uchar* input = (uchar*)malloc(data_size);
 *   uchar* output = (uchar*)malloc(data_size);
 *   cl_mem gpu_input = clCreateBuffer(context, CL_MEM_READ_ONLY, data_size,
 *                                     NULL, &error_code);
 *   cl_mem gpu_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, data_size,
 *                                      NULL, &error_code);
 *   error_code = clEnqueueWriteBuffer(queue, gpu_input, CL_FALSE, 0,
 *                                     data_size, input, 0, NULL, NULL);
 *
 *   equalizeHist(queue, height, width,
 *        width, gpu_input, width, gpu_output);
 *   error_code = clEnqueueReadBuffer(queue, gpu_output, CL_TRUE, 0, data_size,
 *                                    output, 0, NULL, NULL);
 *
 *   free(input);
 *   free(output);
 *   shutDownGpuMemoryPool();
 *   clReleaseMemObject(gpu_input);
 *   clReleaseMemObject(gpu_output);
 *
 *   return 0;
 * }
 * @endcode
 */
ppl::common::RetCode equalizeHist(cl_command_queue queue,
                                  int height,
                                  int width,
                                  int inWidthStride,
                                  const cl_mem inData,
                                  int outWidthStride,
                                  cl_mem outData);

}  // namespace ocl
}  // namespace cv
}  // namespace ppl

#endif  // _ST_HPC_PPL_CV_OCL_CALCHIST_H_
