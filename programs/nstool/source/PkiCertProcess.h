#pragma once
#include <string>
#include <fnd/types.h>
#include <fnd/IFile.h>
#include <fnd/List.h>
#include <fnd/Vec.h>
#include <pki/SignedData.h>
#include <pki/CertificateBody.h>
#include "nstool.h"

class PkiCertProcess
{
public:
	PkiCertProcess();
	~PkiCertProcess();

	void process();

	void setInputFile(fnd::IFile* file, bool ownIFile);
	void setKeyset(const sKeyset* keyset);
	void setCliOutputMode(CliOutputMode type);
	void setVerifyMode(bool verify);

private:
	const std::string kModuleName = "PkiCertProcess";
	static const size_t kSmallHexDumpLen = 0x10;

	fnd::IFile* mFile;
	bool mOwnIFile;
	const sKeyset* mKeyset;
	CliOutputMode mCliOutputMode;
	bool mVerify;

	fnd::List<pki::SignedData<pki::CertificateBody>> mCert;

	void importCerts();
	void validateCerts();
	void displayCerts();
	void displayCert(const pki::SignedData<pki::CertificateBody>& cert);

	size_t getHexDumpLen(size_t max_size) const;
	const char* getSignTypeStr(pki::sign::SignatureId type) const;
	const char* getEndiannessStr(bool isLittleEndian) const;
	const char* getPublicKeyTypeStr(pki::cert::PublicKeyType type) const;
};