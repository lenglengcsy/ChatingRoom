#pragma once
#include "stdafx.h"
#include "sql.h"

CAdoDataBase::CAdoDataBase(void)
{
	m_type=DATABASE_MSSQL;
	m_bFlag=FALSE;
	::CoInitialize(NULL);
	InitializeCriticalSection (&m_Section);
}

CAdoDataBase::~CAdoDataBase(void)
{
	this->Close();
	CoUninitialize();
	DeleteCriticalSection (&m_Section);
}

int CAdoDataBase::Open(const std::string &data)
{
	int ret=0;
	m_strLogin=data;

	this->Lock();
	do 
	{
		try
		{
			m_pConnection.CreateInstance("ADODB.Connection");
			ret=m_pConnection->Open(data.c_str(),"","",adModeUnknown);			
			if (this->m_type == DATABASE_MYSQL)//设置字符集,解决乱码问题
			{				
				m_pConnection->Execute("set character_set_connection=gbk",NULL,adCmdText);
				m_pConnection->Execute("set character_set_results=gbk",NULL,adCmdText);
				m_pConnection->Execute("set character_set_client=gbk",NULL,adCmdText);
			}			
			m_bFlag=TRUE;
			ret=0;
		}
		catch (_com_error e) 
		{
			m_bFlag=FALSE;
			m_strErrCode=e.Description();
			m_pConnection=NULL;
			ret=-1;
		}
	} while (FALSE);
	this->UnLock();
	return ret;
}

int CAdoDataBase::Open(const DATABASE_TYPE &type, const std::string &addr, const std::string &user, \
					   const std::string &pwd, const std::string &database, const std::string &port)
{
	std::string data;
	m_type=type;
	switch(m_type)
	{
	case DATABASE_ACESS:
		data+="Provider=Microsoft.Jet.OLEDB.4.0;";//Microsoft.Ace.Oledb.12.0;
		data+="Data Source="+addr+";";
		data+="Jet OLEDB:Database Password="+pwd;
		break;
	case DATABASE_MYSQL:
		data="DRIVER={MySQL ODBC 3.51 Driver};";
		data+="Server="+addr+";";
		data+="PORT="+port+";";
		data+="USER="+user+";";
		data+="PASSWORD="+pwd+";";
		if(database.length())data+="DATABASE="+database+";";
		break;
	case DATABASE_MSSQL:
		data="Provider=SQLOLEDB.1;Persist Security Info=False;";
		data+="Data Source="+addr+","+port+";";
		data+="User ID="+user+";";
		data+="PassWord="+pwd+";";
		if(database.length())data+="Initial Catalog='"+database+"';";
		break;
	case DATABASE_ORACLE:
		data="Provider=MSDAORA.1;Persist Security Info=False;";
		data+="Server="+addr+","+port+";";
		data+="User ID="+user+";";
		data+="Password="+pwd+";";
		if(database.length())data+="DATABASE="+database+";";
		break;
	case DATABASE_SOURCE:
		data="DSN="+addr+";";
		data+="uid="+user+";";
		data+="pwd="+pwd+";";
		break;
	default:
		m_strErrCode="没有此类型的操作";
		return -1;
	}
	return this->Open(data);
}

int CAdoDataBase::Exectue(const std::string &sql)
{
	int ret=0;

	this->Lock();
	do 
	{
		try
		{
			if(0==m_pConnection)
			{
				if(0!=Open(m_strLogin.c_str()))
				{
					ret=-1;break;
				}
			}
			m_pConnection->Execute(sql.c_str(),NULL,adCmdText);
			m_bFlag=TRUE;
			ret=0;
		}
		catch(_com_error e)
		{
			m_strErrCode=e.Description();
			m_bFlag=FALSE;
			ret=-1;
		}
	} while (FALSE);

	this->UnLock();
	return ret;
}

_RecordsetPtr& CAdoDataBase::GetRecordSet(const _bstr_t &bstrSQL)
{
	try{
		if(0==m_pConnection)
		{
			if(0!=Open(m_strLogin.c_str()))
			{
				return m_pRecordset;
			}
		}
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		m_bFlag=TRUE;
	}
	catch(_com_error e)
	{
		m_strErrCode=e.Description();
		m_bFlag=FALSE;
	}
	return m_pRecordset;
}

int CAdoDataBase::Exectue(const std::string &sql, std::vector<ST_TABLE> &list)
{
	HRESULT		ret		=	-1;
	long		i		=	0;
	long		count	=	0;
	Fields*		fields	=	0;
	BSTR		name	=	0;
	std::string	tmp		=	"";
	ST_TABLE	table;
	_variant_t	value;

	this->Lock();
	do 
	{
		if(sql.length()<1)break;
		
		try
		{
			GetRecordSet(sql.c_str());
			if(FALSE==m_bFlag)break;
			if(0==m_pRecordset)break;
			
			ret=m_pRecordset->get_Fields(&fields);
			if(SUCCEEDED(ret))fields->get_Count(&count);
			for(i=0; i<count; i++)			
			{			
				fields->Item[i]->get_Name(&name);
				table.name= _com_util::ConvertBSTRToString(name);
				list.push_back(table);
			}
			if(SUCCEEDED(ret))fields->Release();
			
			count=list.size();
			while(0!=m_pRecordset && !m_pRecordset->adoEOF)
			{
				for (i=0; i<count; i++)
				{
					value=m_pRecordset->GetCollect(list[i].name.c_str());	
					if(value.vt!=VT_NULL)tmp=(LPCTSTR)(_bstr_t)(value);
					list[i].value.push_back(tmp);
				}
				m_pRecordset->MoveNext();
			}
			m_bFlag=TRUE;
			ret=0;
		}
		catch(_com_error e)
		{
			m_strErrCode=e.Description();
			m_bFlag=FALSE;
			ret=-1;
		}
	} while (FALSE);

	if(m_pRecordset)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
	}
	this->UnLock();
	return ret;
}

void CAdoDataBase::Close(void)
{
	if(m_pRecordset)
	{
		m_pRecordset->Close();
		m_pRecordset=0;
	}
	
	if(m_pConnection)
	{
		m_pConnection->Close();
		m_pConnection=0;
	}
}

std::string CAdoDataBase::GetLastErrCode(void)
{
	return m_strErrCode;
}

_ConnectionPtr CAdoDataBase::GetConnPtr(void)
{
	return m_pConnection;
}


//Acess
CDataBaseAcess::~CDataBaseAcess(void)
{
	m_ado.Close();
}

int CDataBaseAcess::Open(const std::string &data)
{
	return m_ado.Open(data);
}
int CDataBaseAcess::Open(const std::string &addr, const std::string &user, const std::string &pwd, \
						 const std::string &database, const std::string &port)
{
	return m_ado.Open(DATABASE_ACESS,addr,user,pwd,database,port);
}
int CDataBaseAcess::Exectue(const std::string &sql)
{
	return m_ado.Exectue(sql);
}
int CDataBaseAcess::Exectue(const std::string &sql, std::vector<ST_TABLE> &list)
{
	return m_ado.Exectue(sql,list);
}
void CDataBaseAcess::Close(void)
{
	m_ado.Close();
}

std::string CDataBaseAcess::GetLastErrCode(void)
{
	return m_ado.GetLastErrCode();
}

//Mysql
CDataBaseMysql::~CDataBaseMysql(void)
{
	m_ado.Close();
}

int CDataBaseMysql::Open(const std::string &data)
{
	return m_ado.Open(data);
}

int CDataBaseMysql::Open(const std::string &addr, const std::string &user, const std::string &pwd, \
						 const std::string &database, const std::string &port)
{
	std::string m_port=port;
	if(m_port.length()==0)m_port="3306";	
	return m_ado.Open(DATABASE_MYSQL,addr,user,pwd,database,m_port);
}

int CDataBaseMysql::Exectue(const std::string &sql)
{
	return m_ado.Exectue(sql);
}

int CDataBaseMysql::Exectue(const std::string &sql, std::vector<ST_TABLE> &list)
{
	return m_ado.Exectue(sql,list);
}

void CDataBaseMysql::Close(void)
{
	m_ado.Close();
}

std::string CDataBaseMysql::GetLastErrCode(void)
{
	return m_ado.GetLastErrCode();
}


//Mssql
CDataBaseMssql::~CDataBaseMssql(void)
{
	m_ado.Close();
}

int CDataBaseMssql::Open(const std::string &data)
{
	return m_ado.Open(data);
}
int CDataBaseMssql::Open(const std::string &addr, const std::string &user, const std::string &pwd, \
						 const std::string &database, const std::string &port)
{
	std::string m_port=port;
	if(m_port.length()==0)m_port="1433";
	return m_ado.Open(DATABASE_MSSQL,addr,user,pwd,database,m_port);
}
int CDataBaseMssql::Exectue(const std::string &sql)
{
	return m_ado.Exectue(sql);
}

int CDataBaseMssql::Exectue(const std::string &sql, std::vector<ST_TABLE> &list)
{
	return m_ado.Exectue(sql,list);
}

void CDataBaseMssql::Close(void)
{
	m_ado.Close();
}
std::string CDataBaseMssql::GetLastErrCode(void)
{
	return m_ado.GetLastErrCode();
}

//Oracle
CDataBaseOracle::~CDataBaseOracle(void)
{
	m_ado.Close();
}

int CDataBaseOracle::Open(const std::string &data)
{
	return m_ado.Open(data);
}
int CDataBaseOracle::Open(const std::string &addr, const std::string &user, const std::string &pwd, \
						 const std::string &database, const std::string &port)
{
	std::string m_port=port;
	if(m_port.length()==0)m_port="1501";
	return m_ado.Open(DATABASE_ORACLE,addr,user,pwd,database,m_port);
}
int CDataBaseOracle::Exectue(const std::string &sql)
{
	return m_ado.Exectue(sql);
}

int CDataBaseOracle::Exectue(const std::string &sql, std::vector<ST_TABLE> &list)
{
	return m_ado.Exectue(sql,list);
}

void CDataBaseOracle::Close(void)
{
	m_ado.Close();
}
std::string CDataBaseOracle::GetLastErrCode(void)
{
	return m_ado.GetLastErrCode();
}

//Source
CDataBaseSource::~CDataBaseSource(void)
{
	m_ado.Close();
}

int CDataBaseSource::Open(const std::string &data)
{
	return m_ado.Open(data);
}
int CDataBaseSource::Open(const std::string &addr, const std::string &user, const std::string &pwd, \
						  const std::string &database, const std::string &port)
{
	return m_ado.Open(DATABASE_SOURCE,addr,user,pwd,database,port);
}
int CDataBaseSource::Exectue(const std::string &sql)
{
	return m_ado.Exectue(sql);
}

int CDataBaseSource::Exectue(const std::string &sql, std::vector<ST_TABLE> &list)
{
	return m_ado.Exectue(sql,list);
}

void CDataBaseSource::Close(void)
{
	m_ado.Close();
}
std::string CDataBaseSource::GetLastErrCode(void)
{
	return m_ado.GetLastErrCode();
}