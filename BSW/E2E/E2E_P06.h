/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  E2E_P06.h
 *        \brief  E2E header file for AUTOSAR E2E Library Profile 6
 *
 *      \details  E2E protection ensures data exchange which is protected at runtime against the effects of faults within
 *                the communication link. E2E Library provides mechanisms for E2E protection, adequate for safety-related
 *                communication having requirements up to ASIL D.
 *
 *********************************************************************************************************************/

/**** Protection against multiple inclusion **************************************************************************/
#if !defined (E2E_P06_H)
# define E2E_P06_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "E2E.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct {
    uint16 Offset; /*!< Bit offset of the first bit of the E2E header from the beginning */
    uint16 MinDataLength; /*!< Minimal length of Data, in bits */
    uint16 MaxDataLength; /*!< Maximal length of Data, in bits */
    uint16 DataID; /*!< Unique identifier for protection against masquerading */
    uint8 MaxDeltaCounter; /*!< Maximum allowed difference between two counter values of consecutively received valid messages */
} E2E_P06ConfigType;

typedef struct {
    uint8  Counter; /*!< Counter to be used for protecting the next Data */
} E2E_P06ProtectStateType;

typedef enum {
    E2E_P06STATUS_OK = 0x00,
    E2E_P06STATUS_NONEWDATA = 0x01,
    E2E_P06STATUS_ERROR = 0x07,
    E2E_P06STATUS_REPEATED = 0x08,
    E2E_P06STATUS_OKSOMELOST = 0x20,
    E2E_P06STATUS_WRONGSEQUENCE = 0x40
} E2E_P06CheckStatusType;

typedef struct {
    E2E_P06CheckStatusType Status; /*!< Result of verification of the Data in this cycle */
    uint8 Counter; /*!< Counter of the Data in previous cycle */
} E2E_P06CheckStateType;


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define E2E_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  E2E_P06Protect()
 *********************************************************************************************************************/
/*!
 * \brief           Protection function of E2Elib Profile 6
 * \details         Protects the array/buffer to be transmitted using the E2E Profile 6. This includes checksum calculation,
 *                  handling of counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in,out]   DataPtr   Pointer to data to be protected
 * \param[in]       Length    Length of data in bytes
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 * \return          E2E_E_INPUTERR_WRONG: One parameter is erroneous
 * \return          E2E_E_INTERR: Internal library error
 * \return          E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1086
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P06Protect( P2CONST (E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                                P2VAR (E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                                P2VAR (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                                VAR (uint16, AUTOMATIC) Length);


/**********************************************************************************************************************
 *  E2E_P06ProtectInit()
 *********************************************************************************************************************/
/*!
 * \brief           Protection initialization function of E2Elib Profile 6
 * \details         Initializes the protection state by resetting the counter
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1087
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P06ProtectInit( P2VAR (E2E_P06ProtectStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);


/**********************************************************************************************************************
 *  E2E_P06Check()
 *********************************************************************************************************************/
/*!
 * \brief           Check function of E2Elib Profile 6
 * \details         Checks the Data received using the E2E Profile 6. This includes CRC calculation, handling of
 *                  the counter and Data ID.
 * \param[in]       ConfigPtr Pointer to profile configuration
 * \param[in,out]   StatePtr  Pointer to communication state
 * \param[in]       DataPtr   Pointer to data to be protected
 * \param[in]       Length    Length of data in bytes
 * \return          E2E_E_INPUTERR_NULL: A pointer parameter is NULL
 * \return          E2E_E_INPUTERR_WRONG: One parameter is erroneous
 * \return          E2E_E_INTERR: Internal library error
 * \return          E2E_E_OK: Protection successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1088
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P06Check( P2CONST (E2E_P06ConfigType, AUTOMATIC, E2E_APPL_CONST) ConfigPtr,
                                              P2VAR (E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA ) StatePtr,
                                              P2CONST (uint8, AUTOMATIC, E2E_APPL_DATA ) DataPtr,
                                              VAR (uint16, AUTOMATIC) Length);


/**********************************************************************************************************************
 *  E2E_P06CheckInit()
 *********************************************************************************************************************/
/*!
 * \brief           Check initialization function of E2Elib Profile 6
 * \details         Check initialization function of E2Elib Profile 6
 * \param[in,out]   StatePtr  Pointer to communication state
 * \return          E2E_E_INPUTERR_NULL: Pointer parameter is NULL
 * \return          E2E_E_OK: Initialization successful
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1089
 */
FUNC (Std_ReturnType, E2E_CODE) E2E_P06CheckInit( P2VAR (E2E_P06CheckStateType, AUTOMATIC, E2E_APPL_DATA) StatePtr);


/**********************************************************************************************************************
 *  E2E_P06MapStatusToSM()
 *********************************************************************************************************************/
/*!
 * \brief           State mapping function of E2Elib Profile 6
 * \details         Maps the check status of Profile 6 to a generic check status, which can be used by E2E state machine
 *                  check function. The E2E Profile 6 delivers a more fine-grained status, but this is not relevant for
 *                  the E2E state machine.
 * \param[in]       CheckReturn     Return value of E2E_P06Check
 * \param[in]       Status          communication state
 * \return          Standard state value to be used in E2Elib state machine
 * \pre             -
 * \context         TASK
 * \reentrant       FALSE
 * \synchronous     TRUE
 * \trace           CREQ-1175
 * \trace           SPEC-2056606
 */
FUNC (E2E_PCheckStatusType, E2E_CODE) E2E_P06MapStatusToSM( VAR (Std_ReturnType, AUTOMATIC) CheckReturn,
                                                            VAR (E2E_P06CheckStatusType, AUTOMATIC) Status);

# define E2E_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* E2E_P06_H */

/**********************************************************************************************************************
 *  END OF FILE: E2E_P06.h
 *********************************************************************************************************************/
