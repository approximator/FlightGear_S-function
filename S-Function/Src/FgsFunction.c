/*!
 * @file FgsFunction.c
 *
 * @brief Matlab s-function implementation for interacting with FlightGear's FDM.
 * S-function's callbacks reference http://www.mathworks.com/help/simulink/sfg/mdlstart.html
 *
 * @author Oleksii Aliakin (alex@nls.la)
 * @date Created Nov 11, 2014
 * @date Modified Dec 22, 2014
 */

#define S_FUNCTION_NAME  FgsFunction
#define S_FUNCTION_LEVEL 2

#include "transport/FgsTransport.h"
#include "simstruc.h"
#include "log.h"

/*! @todo: use "work vectors" for global variables */
float fdmData[100] = {0, };
FgsTransportContext* transport;

/*!
 * @brief Setup sizes of the various vectors.
 */
static void mdlInitializeSizes(SimStruct *S)
{
    FGS_LOG_INIT(dummy);
    FGS_TRACE("mdlInitializeSizes");
    int_T numInputPorts = 1;
    int_T inputPortWidth = 2;
    int_T numOutputPorts = 1;
    int_T outputPortWidth = 7;

    /** @todo: get params from simulink */
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    {
        return; /* Parameter mismatch will be reported by Simulink */
    }

    ssSetNumContStates(S, 0);   /* number of continuous states           */
    ssSetNumDiscStates(S, 0);   /* number of discrete states             */

    if (!ssSetNumInputPorts(S, numInputPorts))
    {
        return;
    }
    ssSetInputPortWidth(S, 0, inputPortWidth);
    ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, numOutputPorts))
    {
        return;
    }

    ssSetOutputPortWidth(S, 0, outputPortWidth);

    ssSetNumSampleTimes(S, 1);


    /** @todo: complete this stuff wit DWork vectors */
    ssSetNumDWork(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetDWorkWidth(S, 0, 1);
    ssSetDWorkDataType(S, 0, SS_DOUBLE);

    /* Identifier; free any old setting and update */
    /*id = ssGetDWorkRTWIdentifier(S, 0);
    if (id != NULL) {
        free(id);
    }
    id = malloc(80);
    mxGetString(ID_PARAM(S), id, 80);
    ssSetDWorkRTWIdentifier(S, 0, id);*/

    /* Type Qualifier; free any old setting and update */
    /*tq = ssGetDWorkRTWTypeQualifier(S, 0);
    if (tq != NULL) {
        free(tq);
    }
    tq = malloc(80);
    mxGetString(TQ_PARAM(S), tq, 80);
    ssSetDWorkRTWTypeQualifier(S, 0, tq);*/

    /* Storage class */
    /*sc = ((int_T) *((real_T*) mxGetPr(SC_PARAM(S)))) - 1;
    ssSetDWorkRTWStorageClass(S, 0, sc);*/


    /* specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S,
                 SS_OPTION_WORKS_WITH_CODE_REUSE |
                 SS_OPTION_EXCEPTION_FREE_CODE |
                 SS_OPTION_USE_TLC_WITH_ACCELERATOR);
}

#define MDL_INITIALIZE_CONDITIONS
/*!
 * @brief Initialize both continuous states to zero
 */
static void mdlInitializeConditions(SimStruct *S)
{
    real_T *x = (real_T*) ssGetDWork(S,0);

    /* 
     * Initialize the dwork to 0
     */
    /*! @todo complete this stuff wit DWork vectors */
    x[0] = 0.0;
}

#define MDL_START                      /* Change to #undef to remove function */
#if defined(MDL_START)
static void mdlStart(SimStruct *S)
{
    FGS_TRACE("mdlStart");
    transport = fgsCreateTransportContext("0.0.0.0", 5000, "127.0.0.1", 5010);
    if (transport == NULL)
    {
        FGS_TRACE("Can't create transport context.");
        return;
    }

    if (fgsTransportStart(transport) != 0)
    {
        FGS_TRACE("Can't start transport.");
        return;
    }
}
#endif /*  MDL_START */


/*!
 * @brief Specifiy that we inherit our sample time from the driving block.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    FGS_TRACE("mdlInitializeSampleTimes");
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
}

/*!
 * @ brief Read FDM data here and send control signals to FlightGear
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    FGS_TRACE("mdlOutputs");

    /** @todo complete this stuff wit DWork vectors */
    /*real_T *y = ssGetOutputPortRealSignal(S,0);
    real_T *x = (real_T*) ssGetDWork(S,0);

    /* Return the current state as the output */
    /*y[0] = x[0];*/

    int_T             i;
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S, 0);
    real_T            *y    = ssGetOutputPortRealSignal(S, 0);
    int_T             width = ssGetOutputPortWidth(S, 0);
    int_T             inputWidth = ssGetInputPortWidth(S, 0);

    char recv_data[1024];
    /* data to read from flightgear */
    const int HEADING   = 0;
    const int PITCH     = 1;
    const int ROLL      = 2;
    const int YAW       = 3;
    const int ALTITUDE  = 4;
    const int LATITUDE  = 5;
    const int LONGITUDE = 6;

    do
    {
        int err;
        if (!fgsTransportIsDataToRead(transport, 1))
        {
            FGS_TRACE("There is no data to read...");
            break;
        }

        FGS_TRACE("Reading data");
        int bytes_read = fgsTransportReadData(transport, recv_data, 1024);
        recv_data[bytes_read] = '\0';
        FGS_TRACE("Read %d bytes.", bytes_read);
        FGS_TRACE("%s", recv_data);

        sscanf(recv_data, "%f %f %f %f %f %f %f\n",
          &fdmData[HEADING], &fdmData[PITCH], &fdmData[ROLL],
          &fdmData[ALTITUDE], &fdmData[YAW], &fdmData[LATITUDE],
          &fdmData[LONGITUDE]);

        /* Construct a packet to send over UDP with control inputs     */
        char send_data[1024];
        real_T aileron_ctrl, pitch_ctrl;
        aileron_ctrl = *uPtrs[0];
        pitch_ctrl = *uPtrs[1];

        int msgL = sprintf(send_data, "%f\t%f\n", aileron_ctrl, pitch_ctrl); 
        FGS_TRACE("send_data = %s", send_data);
        /* Send UDP packet */
        fgsTransportWriteData(transport, send_data, strlen(send_data));
    } while(0);

    /* Put FDM data to output ports */
    y[0] = fdmData[HEADING];
    y[1] = fdmData[PITCH];
    y[2] = fdmData[ROLL];
    y[3] = fdmData[ALTITUDE];
    y[4] = fdmData[YAW];
    y[5] = fdmData[LATITUDE];
    y[6] = fdmData[LONGITUDE];
}


/*!
 * @brief Free all used resources.
 */
static void mdlTerminate(SimStruct *S)
{
    FGS_TRACE("mdlTerminate");
    fgsTransportDestroy(transport);
    FGS_TRACE("Transport destroy\n\n\n\n\n\n\n\n\n\n\n\n\n");


    char* id;
    char* tq;

    /** @todo complete this stuff wit DWork vectors */
    /* Identifier; free any old setting and update */
    id = ssGetDWorkRTWIdentifier(S, 0);
    if (id != NULL) {
        free(id);
    }
    ssSetDWorkRTWIdentifier(S, 0, NULL);

    /* Type Qualifier; free any old setting and update */
    tq = ssGetDWorkRTWTypeQualifier(S, 0);
    if (tq != NULL) {
        free(tq);
    }
    ssSetDWorkRTWTypeQualifier(S, 0, NULL);

    FGS_LOG_STOP(dummy);
}



#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
