/****************************************************************************
 ** Main author: msr                     Creation date: 03/21/06
 ** $LastChangedBy: mns $
 ** $Date: 2008-11-05 18:23:56 +0100 (Mi, 05. Nov 2008) $
 ** $URL: https://scm/svn/library-ES/branches/library-es/1.2.0-VectorInformatik/development/common/lib_es/source/ESLib_RSA_V15_SigCRT_SHA1.c $
 ** $Rev: 1757 $
 ** $NoKeywords$
 **
 **
 ** \copyright(cv cryptovision GmbH, 1999 - 2006)
 **
 ***************************************************************************/

/****************************************************************************
 **
 **     This file is part of the embedded systems library cvActLib/ES.
 **
 **     Layer: core level
 **
 ***************************************************************************/

/****************************************************************************
 **
 ** This file contains: RSA V1.5 (Sign_SHA1/Verify_SHA1) implementation file.
 **
 ** constants:
 **
 ** types:
 **
 ** macros:
 **
 ** functions:
 **    esl_initSignRSACRTSHA1_V15
 **    esl_updateSignRSACRTSHA1_V15
 **    esl_finalizeSignRSACRTSHA1_V15
 **
 ***************************************************************************/


#include "ESLib.h"
#include "ESLib_types.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"
#include "actUtilities.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

CRYPTOCV_CONST( STATIC, eslt_Byte ) eslt_ASN1_DIGESTINFO_SHA1[] = ASN1_DIGESTINFO_SHA1;

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_initSignRSACRTSHA1_V15(       eslt_WorkSpaceRSACRTsig *workSpace,
 **                                    eslt_Length              keyPairPrimePSize,
 **                              const eslt_Byte               *keyPairPrimeP,
 **                                    eslt_Length              keyPairPrimeQSize,
 **                              const eslt_Byte               *keyPairPrimeQ,
 **                                    eslt_Length              privateKeyExponentDPSize,
 **                              const eslt_Byte               *privateKeyExponentDP,
 **                                    eslt_Length              privateKeyExponentDQSize,
 **                              const eslt_Byte               *privateKeyExponentDQ,
 **                                    eslt_Length              privateKeyInverseQISize,
 **                              const eslt_Byte               *privateKeyInverseQI)
 **
 ** input:
 ** - workSpace
 ** - keyPairPrimePSize
 ** - keyPairPrimeP
 ** - keyPairPrimeQSize
 ** - keyPairPrimeQ
 ** - privateKeyExponentDPSize
 ** - privateKeyExponentDP
 ** - privateKeyExponentDQSize
 ** - privateKeyExponentDQ
 ** - privateKeyInverseQISize
 ** - privateKeyInverseQI
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_TOO_SMALL
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **      ESL_ERC_PRIVKEY_INVALID
 **      ESL_ERC_RSA_MODULE_OUT_OF_RANGE
 **      - keyPairModuleSize < 46d = headLen+hLen+11, where
 **           + hLen is the length of the Hashvalue (SHA-1: 20d= 0x14)
 **           + headLen is the AlgoIDentifier (RSA+SHA1: 15d=0x0f)
 **
 ** assumes:
 ** - PrimeP and PrimeQ are a prime
 ** - privateKeyExpDP<PrimeP
 ** - privateKeyExpDQ<PrimeQ
 ** - QI is inverse to Q, could be checked, then move this line to errorcodes
 ** - the Hash-Function is SHA-1
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_initSignRSACRTSHA1_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig ) workSpace,
   eslt_Length keyPairPrimePSize, eslt_pRomByte keyPairPrimeP,
   eslt_Length keyPairPrimeQSize, eslt_pRomByte keyPairPrimeQ,
   eslt_Length privateKeyExponentDPSize, eslt_pRomByte privateKeyExponentDP,
   eslt_Length privateKeyExponentDQSize, eslt_pRomByte privateKeyExponentDQ,
   eslt_Length privateKeyInverseQISize, eslt_pRomByte privateKeyInverseQI )
{

    eslt_ErrorCode               returnValue;
    eslt_Length                  keyPairModuleSize;
    CRYPTOCV_P2VAR_PARA( eslt_Byte                    ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader         ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1           ) wsSHA1;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig_prim ) wsPRIM;

    returnValue = ESL_ERC_NO_ERROR;

    if ( !workSpace )
    {
        returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
        /* get underlying workspace pointer */
        wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsHeaderV15       = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
        keyPairModuleSize = (eslt_Length)(keyPairPrimePSize + keyPairPrimeQSize);
        wsSHA1            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ))(wsRAW + ESL_WS_RSA_V15_SV_SHA1_WS_SHA1(keyPairModuleSize));
        wsPRIM            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig_prim ))(wsRAW + ESL_WS_RSA_V15_SV_SHA1_WS_RSA_PRIM(keyPairModuleSize));

        if ( wsHeaderV15->size < (ESL_SIZEOF_WS_RSA_V15_SV_SHA1_OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)) )
        {
            returnValue = ESL_ERC_WS_TOO_SMALL;
        }
        else
        {
            if (keyPairModuleSize < 46)
            {
                returnValue = ESL_ERC_RSA_MODULE_OUT_OF_RANGE;
            }
            else
            {
                wsSHA1->header.size = (eslt_Length)(sizeof(eslt_WorkSpaceSHA1) - sizeof(eslt_WorkSpaceHeader));
                wsSHA1->header.watchdog = wsHeaderV15->watchdog;
                returnValue = esl_initSHA1(wsSHA1);
                if (ESL_ERC_NO_ERROR == returnValue)
                {
                    wsPRIM->header.size = (eslt_Length)(wsHeaderV15->size - (ESL_SIZEOF_WS_RSA_V15_SV_SHA1_OVERHEAD(keyPairModuleSize) + sizeof(eslt_WorkSpaceHeader)));
                    wsPRIM->header.watchdog = wsHeaderV15->watchdog;
                    returnValue =  esl_initSignRSACRT_prim(wsPRIM,
                    keyPairPrimePSize, keyPairPrimeP,
                    keyPairPrimeQSize, keyPairPrimeQ,
                    privateKeyExponentDPSize, privateKeyExponentDP,
                    privateKeyExponentDQSize, privateKeyExponentDQ,
                    privateKeyInverseQISize, privateKeyInverseQI);
                }
            }
        }
        if ( ESL_ERC_NO_ERROR == returnValue )
        {
            wsHeaderV15->status = (ESL_WST_ALGO_RSA | ESL_WST_M_RUNNING | ESL_WST_M_CRITICAL);
            /* store message buffer length in workspace */
            wsRAW[ESL_WS_RSA_V15_SV_SHA1_BUFFERLENGTH]     = (eslt_Byte)(keyPairModuleSize >> 8);
            wsRAW[ESL_WS_RSA_V15_SV_SHA1_BUFFERLENGTH + 1] = (eslt_Byte)(keyPairModuleSize);
        }
    }
    return returnValue;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_updateSignRSACRTSHA1_V15(       eslt_WorkSpaceRSACRTsig *workSpace,
 **                                const eslt_Length              inputSize,
 **                                const eslt_Byte               *input)
 **
 ** input:
 ** - workSpace
 ** - inputSize
 ** - input
 **
 ** output:
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID
 **      ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_updateSignRSACRTSHA1_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig ) workSpace,
   const eslt_Length inputSize, CRYPTOCV_P2CONST_PARA( eslt_Byte ) input )
{

    eslt_ErrorCode returnValue;
    eslt_Length    keyPairModuleSize;
    CRYPTOCV_P2VAR_PARA( eslt_Byte            ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1   ) wsSHA1;

    returnValue = ESL_ERC_NO_ERROR;

    if ( !workSpace )
    {
        returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
        /* get underlying workspace pointer */
        wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsHeaderV15       = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
        keyPairModuleSize = (eslt_Length)(((eslt_Length)wsRAW[ESL_WS_RSA_V15_SV_SHA1_BUFFERLENGTH] << 8) + (wsRAW[ESL_WS_RSA_V15_SV_SHA1_BUFFERLENGTH + 1]));
        wsSHA1            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ))(wsRAW + ESL_WS_RSA_V15_SV_SHA1_WS_SHA1(keyPairModuleSize));

        if ( ESL_WST_ALGO_RSA != (wsHeaderV15->status & ESL_WST_M_ALGO) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
        else if ( 0 == (wsHeaderV15->status & ESL_WST_M_RUNNING) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
        else
        {
            returnValue = esl_updateSHA1( wsSHA1, inputSize, input );
        }
    }
    return returnValue;
}


/****************************************************************************
 **
 ** FUNCTION:
 **  eslt_ErrorCode
 **  esl_finalizeSignRSACRTSHA1_V15( eslt_WorkSpaceRSACRTsig *workSpace,
 **                                  eslt_Length             *signatureSize,
 **                                  eslt_Byte               *signature)
 **
 ** input:
 ** - workSpace
 **
 ** output:
 ** - signatureSize
 ** - signature
 ** - errorCode:
 **      ESL_ERC_NO_ERROR
 **      ESL_ERC_WS_STATE_INVALID
 **      ESL_ERC_PARAMETER_INVALID  - format problems, nullpointer, ...
 **      ESL_ERC_OUTPUT_SIZE_TOO_SHORT
 **
 ** assumes:
 **
 ***************************************************************************/
CRYPTOCV_FUNC( eslt_ErrorCode ) esl_finalizeSignRSACRTSHA1_V15(
   CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig ) workSpace,
   CRYPTOCV_P2VAR_PARA( eslt_Length ) signatureSize, CRYPTOCV_P2VAR_PARA( eslt_Byte ) signature )
{

    eslt_ErrorCode returnValue;
    eslt_Length    keyPairModuleSize, i, padBytesNeeded;
    CRYPTOCV_P2VAR_PARA( eslt_Byte                    ) wsRAW;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1           ) wsSHA1;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig_prim ) wsPRIM;
    CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader         ) wsHeaderV15;
    CRYPTOCV_P2VAR_PARA( eslt_Byte                    ) messageBuf;

    returnValue = ESL_ERC_NO_ERROR;

    if ( !workSpace )
    {
        returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
        i                 = 0;
        /* get underlying workspace pointer */
        wsRAW             = (CRYPTOCV_P2VAR_PARA( eslt_Byte ))workSpace;
        wsHeaderV15       = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceHeader ))wsRAW;
        keyPairModuleSize = (eslt_Length)(((eslt_Length)wsRAW[ESL_WS_RSA_V15_SV_SHA1_BUFFERLENGTH] << 8) + (wsRAW[ESL_WS_RSA_V15_SV_SHA1_BUFFERLENGTH + 1]));
        wsSHA1            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceSHA1 ))(wsRAW + ESL_WS_RSA_V15_SV_SHA1_WS_SHA1(keyPairModuleSize));
        wsPRIM            = (CRYPTOCV_P2VAR_PARA( eslt_WorkSpaceRSACRTsig_prim ))(wsRAW + ESL_WS_RSA_V15_SV_SHA1_WS_RSA_PRIM(keyPairModuleSize));
        messageBuf        = wsRAW + ESL_WS_RSA_V15_SV_SHA1_BUFFER;

        keyPairModuleSize = (eslt_Length)( ((CRYPTOCV_P2VAR_PARA( actRSACRTSTRUCT ))wsPRIM->wsRSA)->n_bytes );
        padBytesNeeded    = (eslt_Length)(keyPairModuleSize - ASN1_SIZEOF_PARAMETERS_WITH_FIXED_LENGTH);

        if ( ESL_WST_ALGO_RSA != (wsHeaderV15->status & ESL_WST_M_ALGO) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
        else if ( 0 == (wsHeaderV15->status & ESL_WST_M_RUNNING) )
        {
            returnValue = ESL_ERC_WS_STATE_INVALID;
        }
        else if ( keyPairModuleSize < ASN1_MINIMAL_SIGVER_KEY_LENGTH )
        {
            returnValue = ESL_ERC_PARAMETER_INVALID;
        }
        else
        {
            messageBuf[i++] = 0x00;
            messageBuf[i++] = 0x01;
            if ( padBytesNeeded < ASN1_MINIMAL_PADDING_LENGTH )
            {
                returnValue = ESL_ERC_PARAMETER_INVALID;
            }
            else
            {
                actMemset( (CRYPTOCV_P2VAR_PARA( actU8 ))(messageBuf + i),
                           0xff, (unsigned int) padBytesNeeded );
                i = (eslt_Length)(padBytesNeeded + i);
                messageBuf[i++] = 0x00;
                actMemcpy_ROMRAM( (CRYPTOCV_P2VAR_PARA( actU8 ))(messageBuf + i),
                                  eslt_ASN1_DIGESTINFO_SHA1, sizeof(eslt_ASN1_DIGESTINFO_SHA1) );
                i += sizeof(eslt_ASN1_DIGESTINFO_SHA1);
                returnValue = esl_finalizeSHA1( wsSHA1, messageBuf + i );

                if ( ESL_ERC_NO_ERROR == returnValue )
                {
                    returnValue = esl_signRSACRT_prim( wsPRIM, keyPairModuleSize, messageBuf, signatureSize, signature );
                }
            }
        }
    }
    return returnValue;
}


