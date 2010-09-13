#include "mex.h"
#include "opencv_matlab_interop.h"

#define PRINT_INPUTS

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    ASSERT_NUM_LHS_ARGS_EQUALS(1);
    ASSERT_NUM_RHS_ARGS_EQUALS(2);

    //retrieve the pointer to the random forest
    CvRTrees *forest = (CvRTrees *)unpack_pointer(prhs[0]);

    //get the data which we need to predict on into opencv format
    CvMat* dataMtx = matlab_matrix_to_opencv_matrix(prhs[1]);
    CvMat sample; //this is used to point to each row, one at a time

    int numSamples = dataMtx->rows;
    mexPrintf("predicting on %d samples\n", numSamples);

    mxArray* output = mxCreateDoubleMatrix(numSamples, 1, mxREAL);
    double *outputData = (double *)mxGetPr(output);

    //
    for (unsigned int i=0; i<numSamples; i++) {
        cvGetRow(dataMtx, &sample, i);
        outputData[i] = (double)forest->predict(&sample);
    }

    plhs[0] = output;
    cvReleaseMat(&dataMtx);
}
