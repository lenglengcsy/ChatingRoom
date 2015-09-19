#ifndef __DATABASE_H__
#define __DATABASE_H__

#pragma warning(disable:4786)

#include <string>
#include <vector>
using namespace std;
#import "msado15.dll" \
	no_namespace \
rename ("EOF", "adoEOF")

typedef struct __ST_TABLE__
{
	std::string name;
	std::vector<std::string> value;
}ST_TABLE;

typedef enum
{
	DATABASE_START,
	DATABASE_ACESS,
	DATABASE_MYSQL,
	DATABASE_MSSQL,
	DATABASE_ORACLE,
	DATABASE_SOURCE,
	DATABASE_NULL,
	DATABSE_END
}DATABASE_TYPE;

class CAdoDataBase
{
public:
	CAdoDataBase(void);
	virtual ~CAdoDataBase(void);
	
public:
	int Open(const std::string &data);
	int Open(const DATABASE_TYPE &type, const std::string &addr, \
		const std::string &user="", const std::string &pwd="", const std::string &database="", const std::string &port="");
	int Exectue(const std::string &sql);
	int Exectue(const std::string &sql, std::vector<ST_TABLE> &list);
	void Close(void);

	std::string GetLastErrCode(void);

	_ConnectionPtr GetConnPtr(void);
private:
	_RecordsetPtr& GetRecordSet(const _bstr_t &bstrSQL);
	void Lock(void){EnterCriticalSection (&m_Section);}
	void UnLock(void){LeaveCriticalSection (&m_Section);}
private:
	BOOL			m_bFlag;
	DATABASE_TYPE	m_type;
	std::string		m_strLogin;
	std::string		m_strErrCode;

	_RecordsetPtr	m_pRecordset;
	_ConnectionPtr	m_pConnection;

	CRITICAL_SECTION m_Section;
};

class CDataBase
{
public:
	virtual int Open(const std::string &data)=0;
	virtual int Open(const std::string &addr, const std::string &user, \
		const std::string &pwd, const std::string &database, const std::string &port="")=0;
	
	virtual int Exectue(const std::string &sql)=0;
	virtual int Exectue(const std::string &sql, std::vector<ST_TABLE> &list)=0;
	virtual void Close(void)=0;
	virtual std::string GetLastErrCode(void)=0;
};

class CDataBaseAcess:public CDataBase
{
public:
	virtual ~CDataBaseAcess(void);
	
public:
	int Open(const std::string &data);
	int Open(const std::string &addr, const std::string &user, const std::string &pwd, \
		const std::string &database, const std::string &port="");
	
	int Exectue(const std::string &sql);
	int Exectue(const std::string &sql, std::vector<ST_TABLE> &list);
	void Close(void);
	std::string GetLastErrCode(void);
	
private:
	CAdoDataBase m_ado;
};

class CDataBaseMysql:public CDataBase
{
public:
	virtual ~CDataBaseMysql(void);
	
public:
	int Open(const std::string &data);
	int Open(const std::string &addr, const std::string &user, const std::string &pwd, \
		const std::string &database, const std::string &port="3306");
	
	int Exectue(const std::string &sql);
	int Exectue(const std::string &sql, std::vector<ST_TABLE> &list);
	void Close(void);
	std::string GetLastErrCode(void);
	
private:
	CAdoDataBase m_ado;
};

class CDataBaseMssql:public CDataBase
{
public:
	virtual ~CDataBaseMssql(void);
	
public:
	int Open(const std::string &data);
	int Open(const std::string &addr, const std::string &user, const std::string &pwd, \
		const std::string &database, const std::string &port="1433");
	
	int Exectue(const std::string &sql);
	int Exectue(const std::string &sql, std::vector<ST_TABLE> &list);
	void Close(void);
	std::string GetLastErrCode(void);
	
private:
	CAdoDataBase m_ado;
};

class CDataBaseOracle:public CDataBase
{
public:
	virtual ~CDataBaseOracle(void);
	
public:
	int Open(const std::string &data);
	int Open(const std::string &addr, const std::string &user, const std::string &pwd, \
		const std::string &database, const std::string &port="1501");
	
	int Exectue(const std::string &sql);
	int Exectue(const std::string &sql, std::vector<ST_TABLE> &list);
	void Close(void);
	std::string GetLastErrCode(void);
	
private:
	CAdoDataBase m_ado;
};

class CDataBaseSource:public CDataBase
{
public:
	virtual ~CDataBaseSource(void);
	
public:
	int Open(const std::string &data);
	int Open(const std::string &addr, const std::string &user, const std::string &pwd, \
		const std::string &database, const std::string &port="1501");
	
	int Exectue(const std::string &sql);
	int Exectue(const std::string &sql, std::vector<ST_TABLE> &list);
	void Close(void);
	std::string GetLastErrCode(void);
	
private:
	CAdoDataBase m_ado;
};
#endif