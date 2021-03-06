#include "stdafx.h"
#include "VTSNightFuturesCurrent.h"
#include "NC0.h"
#include "t2801.h"
#include "../VTS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


VTSNightFuturesCurrent::VTSNightFuturesCurrent(void) 
{
}

VTSNightFuturesCurrent::~VTSNightFuturesCurrent(void)
{
	SER()->UnregisterCurrent(m_pCode, mt_CME, this);
}

BOOL VTSNightFuturesCurrent::RunRecv()
{
	int nRet = SER()->RegisterCurrent(m_pCode, mt_CME, this);

	if(nRet == 1)
		return TRUE;
	else if(nRet == 0)
	{
		//이미 다른창이 떠있는 것이다
		NotifyInit();
		return TRUE;
	}
	return FALSE;
}

void VTSNightFuturesCurrent::StopRecv()
{
	SER()->UnregisterCurrent(m_pCode, mt_CME, this);
}

BOOL VTSNightFuturesCurrent::OnInitReceive(CHAR* pPacket, UINT nDataLength)
{
	if(nDataLength != sizeof(t2801OutBlock))
		return FALSE;

	LPt2801OutBlock p = (LPt2801OutBlock)pPacket;
	
	GTXD(hname         , string		); //  한글명                      
    GTXD(price         , float		); //  현재가                      
    GTXD(sign          , string		); //  전일대비구분                 
    GTXD(change        , float		); //  전일대비                     
    GTXD(jnilclose     , float		); //  전일종가
	drate.Set(p->diff, sizeof(p->diff), xdt_float);
    GTXD(volume        , long		); //  거래량                      
    GTXD(value         , long		); //  거래대금
	openyak.Set(p->mgjv, sizeof(p->mgjv), xdt_long);
	openyakcha.Set(p->mgjvdiff, sizeof(p->mgjvdiff), xdt_long);
    //GTXD(mgjv          , long		); //  미결제량                     
    //GTXD(mgjvdiff      , long		); //  미결제증감                   
    GTXD(open          , float		); //  시가                        
    GTXD(high          , float		); //  고가                        
    GTXD(low           , float		); //  저가                        
    GTXD(uplmtprice    , float		); //  상한가                      
    GTXD(dnlmtprice    , float		); //  하한가                      
    GTXD(high52w       , float		); //  52최고가                    
    GTXD(low52w        , float		); //  52최저가                    
    GTXD(basis         , float		); //  베이시스                     
    GTXD(recprice      , float		); //  기준가                      
    GTXD(theoryprice   , float		); //  이론가                      
    GTXD_FD(glyl       , 3); //  괴리율
    GTXD(cbhprice      , float		); //  CB상한가                    
    GTXD(cblprice      , float		); //  CB하한가                    
    GTXD(lastmonth     , string		); //  만기일                      
    GTXD(jandatecnt    , long		); //  잔여일                      
    GTXD(pricejisu     , float		); //  종합지수                     
    GTXD(jisusign      , string		); //  종합지수전일대비구분          
    GTXD(jisuchange    , float		); //  종합지수전일대비              
    GTXD(jisudiff      , float		); //  종합지수등락율
	k200jisu.Set(p->kospijisu, sizeof(p->kospijisu), xdt_float);
    //GTXD(kospijisu     , float		); //  KOSPI200지수                
    GTXD(kospisign     , string		); //  KOSPI200전일대비구분         
    GTXD(kospichange   , float		); //  KOSPI200전일대비             
    GTXD(kospidiff     , float		); //  KOSPI200등락율              
    GTXD(listhprice    , float		); //  상장최고가                   
    GTXD(listlprice    , float		); //  상장최저가                   
    GTXD(sbasis        , float		); //  시장BASIS                   
    GTXD(ibasis        , float		); //  이론BASIS                   
    //GTXD(danhochk      , string		); //  단일가호가여부                
    //GTXD(yeprice       , float		); //  예상체결가                   
    //GTXD(jnilysign     , string		); //  예상체결가전일종가대비구분     
    //GTXD(jnilychange   , float		); //  예상체결가전일종가대비         
    //GTXD(jnilydrate    , float		); //  예상체결가전일종가등락율       
    //GTXD(alloc_gubun   , string		); //  배분구분(1:배분개시2:배분해제0:

	NotifyInit();

	m_lastPrice = price.ToString();
	return TRUE;
}

BOOL VTSNightFuturesCurrent::OnReceive(CHAR* pPacket, UINT nDataLength)
{
	LPNC0_OutBlock p = (LPNC0_OutBlock)pPacket;
	
	GTXD(chetime    , time);    // 체결시간          
	GTXD(sign       , string);    // 전일대비구분      
	GTXD(change     , float );    // 전일대비          
	GTXD(drate      , float );    // 등락율            
	GTXD(price      , float );    // 현재가            
	GTXD(open       , float );    // 시가              
	GTXD(high       , float );    // 고가              
	GTXD(low        , float );    // 저가              
	GTXD(cgubun     , string);    // 체결구분          
	GTXD(cvolume    , long  );    // 체결량 
#if 0
	GTXD(volume     , long  );    // 누적거래량        
	GTXD(value      , long  );    // 누적거래대금      
	GTXD(mdvolume   , long  );    // 매도누적체결량    
	GTXD(mdchecnt   , long  );    // 매도누적체결건수  
	GTXD(msvolume   , long  );    // 매수누적체결량    
	GTXD(mschecnt   , long  );    // 매수누적체결건수  
	GTXD(cpower     , float );    // 체결강도          
	GTXD(offerho1   , float );    // 매도호가1         
	GTXD(bidho1     , float );    // 매수호가1         
	GTXD(openyak    , long  );    // 미결제약정수량    
	GTXD(k200jisu   , float );    // KOSPI200지수      
	GTXD(theoryprice, float );    // 이론가            
	GTXD(kasis      , float );    // 괴리율            
	GTXD(sbasis     , float );    // 시장BASIS         
	GTXD(ibasis     , float );    // 이론BASIS         
	GTXD(openyakcha , long  );    // 미결제약정증감    
	GTXD(jgubun     , string);    // 장운영정보        
	GTXD(jnilvolume , long  );    // 전일동시간대거래량
	//GTXD(futcode    , string);    // 단축코드          
#endif

	AddContract(::GetStockTimeStr(chetime.ToINT()), price.ToString(), cvolume.ToString(), p->cgubun[0] == _T('-') ? PT_SELL : PT_BUY);
	return TRUE;
}


