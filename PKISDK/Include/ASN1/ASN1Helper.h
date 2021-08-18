#ifndef __ASN1_HELPER_H__
#define __ASN1_HELPER_H__

/**
  * @Name: ASN1Helper.h
  * @Copyright: KTXA SoftWare
  * @Author: Hongwei Yu
  * @Date:  2003/04/08 18:27
  * @Description: all asn1  helper class and macro 
*/

#ifdef KT_WIN32
	#pragma pack(push, 1)
#endif

#include "ASN1.h"

#define PLACE1OBJ(obj1) \
	m_ppObj[0] = &obj1;

#define PLACE2OBJS(obj1, obj2) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;

#define PLACE3OBJS(obj1, obj2, obj3) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;

#define PLACE4OBJS(obj1, obj2, obj3, obj4) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;

#define PLACE5OBJS(obj1, obj2, obj3, obj4, obj5) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;

#define PLACE6OBJS(obj1, obj2, obj3, obj4, obj5, obj6) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;

#define PLACE7OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;

#define PLACE8OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;

#define PLACE9OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;

#define PLACE10OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;

#define PLACE11OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;\
	m_ppObj[10] = &obj11;

#define PLACE12OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;\
	m_ppObj[10] = &obj11;\
	m_ppObj[11] = &obj12;

#define PLACE13OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;\
	m_ppObj[10] = &obj11;\
	m_ppObj[11] = &obj12;\
	m_ppObj[12] = &obj13;
	
#define PLACE14OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;\
	m_ppObj[10] = &obj11;\
	m_ppObj[11] = &obj12;\
	m_ppObj[12] = &obj13;\
	m_ppObj[13] = &obj14;

#define PLACE15OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;\
	m_ppObj[10] = &obj11;\
	m_ppObj[11] = &obj12;\
	m_ppObj[12] = &obj13;\
	m_ppObj[13] = &obj14;\
	m_ppObj[14] = &obj15;

#define PLACE24OBJS(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24) \
	m_ppObj[0] = &obj1;\
	m_ppObj[1] = &obj2;\
	m_ppObj[2] = &obj3;\
	m_ppObj[3] = &obj4;\
	m_ppObj[4] = &obj5;\
	m_ppObj[5] = &obj6;\
	m_ppObj[6] = &obj7;\
	m_ppObj[7] = &obj8;\
	m_ppObj[8] = &obj9;\
	m_ppObj[9] = &obj10;\
	m_ppObj[10] = &obj11;\
	m_ppObj[11] = &obj12;\
	m_ppObj[12] = &obj13;\
	m_ppObj[13] = &obj14;\
	m_ppObj[14] = &obj15;\
	m_ppObj[15] = &obj16;\
	m_ppObj[16] = &obj17;\
	m_ppObj[17] = &obj18;\
	m_ppObj[18] = &obj19;\
	m_ppObj[19] = &obj20;\
	m_ppObj[20] = &obj21;\
	m_ppObj[21] = &obj22;\
	m_ppObj[22] = &obj23;\
	m_ppObj[23] = &obj24;

#define OptionPlacedObjs(n)\
	for(int i=0; i<n; i++)\
		m_ppObj[i]->setOptional();

#define ChoiceFirstPlacedObj()\
	choiceObj(m_ppObj[0]);

#define SetTypeExPlacedObjs(n, isExplicit)\
	for(int j=0; j<n; j++)\
		m_ppObj[j]->setTypeEx(j, isExplicit);

#define DECLARE_CLASS_KTSEQUENCE(n) \
class KT_LIB_MODE KTSEQUENCE##n : public KTSequence\
{\
public:\
	int  modifyLen();\
protected:\
	int  createContent();\
	void outputContentIn() const;\
protected:\
    KTObject *m_ppObj[n];\
};

DECLARE_CLASS_KTSEQUENCE(1)
DECLARE_CLASS_KTSEQUENCE(2)
DECLARE_CLASS_KTSEQUENCE(3)
DECLARE_CLASS_KTSEQUENCE(4)
DECLARE_CLASS_KTSEQUENCE(5)
DECLARE_CLASS_KTSEQUENCE(6)
DECLARE_CLASS_KTSEQUENCE(7)
DECLARE_CLASS_KTSEQUENCE(8)
DECLARE_CLASS_KTSEQUENCE(9)
DECLARE_CLASS_KTSEQUENCE(10)
DECLARE_CLASS_KTSEQUENCE(11)
DECLARE_CLASS_KTSEQUENCE(12)
DECLARE_CLASS_KTSEQUENCE(13)
DECLARE_CLASS_KTSEQUENCE(14)
DECLARE_CLASS_KTSEQUENCE(15)

#define DECLARE_CLASS_KTCHOICE(n) \
class KT_LIB_MODE KTCHOICE##n : public KTChoice\
{\
protected:\
	KTObject *m_ppObj[n];\
protected:\
	int  createContent();\
};

DECLARE_CLASS_KTCHOICE(1)
DECLARE_CLASS_KTCHOICE(2)
DECLARE_CLASS_KTCHOICE(3)
DECLARE_CLASS_KTCHOICE(4)
DECLARE_CLASS_KTCHOICE(5)
DECLARE_CLASS_KTCHOICE(6)
DECLARE_CLASS_KTCHOICE(7)
DECLARE_CLASS_KTCHOICE(8)
DECLARE_CLASS_KTCHOICE(9)
DECLARE_CLASS_KTCHOICE(10)
DECLARE_CLASS_KTCHOICE(11)
DECLARE_CLASS_KTCHOICE(12)
DECLARE_CLASS_KTCHOICE(13)
DECLARE_CLASS_KTCHOICE(14)
DECLARE_CLASS_KTCHOICE(15)
DECLARE_CLASS_KTCHOICE(24)

#define ANYOF(KTObjSequence, KTObj, KTANY) \
class KT_LIB_MODE KTObjSequence : public KTANY\
{\
public:\
	virtual KTObject* newObj() const { return new KTObj; }\
	KTObj* increaseNode(int nIndex=-1) { return (KTObj*) KTANY::increaseNode(nIndex); } \
    const KTObj* indexObj(int nIndex) const { return (const KTObj*) KTANY::indexObj(nIndex); }\
};

#define SETOF(KTObjSet, KTObj) ANYOF(KTObjSet, KTObj, KTSetOf)
#define SEQUENCEOF(KTObjSequence, KTObj) ANYOF(KTObjSequence, KTObj, KTSequenceOf)

#define KTOBJ_OUTPUT_BYTEARR(obj, rBuf) \
	rBuf.resize(obj.totalCount());\
	obj.output(&rBuf[0]);

////////////////////////////////////////////////////////////////////////////////
////KTTWin
class KT_LIB_MODE KTTwin : public KTSequence
{
public:
	KTTwin() { m_pValue=NULL; };
	virtual ~KTTwin() { delete m_pValue; };
	
	KTBOOL isValueExist() const { return m_pValue!=NULL; };	
	const KTObjectIdentifier& typeObj() const { return m_type; };
	const KTObject* valueObj() const { return m_pValue; };
	
	int setTypeValue(const char* pc_objID, const KTObject* pObj=NULL);
	int setTypeCreateValue(const char* pc_objID, const BYTE* pbByte, UINT unLen);

	int	modifyLen();

protected:
	virtual KTObject* newObj(const char* pc_typeId) const =0;	
	int createContent();
	void outputContentIn() const;

	KTObjectIdentifier		m_type;
	KTObject*			    m_pValue;
};

#ifdef KT_WIN32
	#pragma pack(pop)
#endif

/// Rivison history
/// ----------------------------------------------------------------------------
/// 	Version	       Date        Author      Note
/// ----------------------------------------------------------------------------
///   1.0.0.0  2003/04/08 19:23  Hongwei Yu   created 
///   1.0.0.1  2003/04/09 16:50  Hongwei Yu   choice support
///   1.0.0.2  2003/05/13 02:32  Hongwei Yu   remove choice constutor
///   1.0.0.3  2003/05/14 02:32  Hongwei Yu   refactory KTTWin

#endif  
