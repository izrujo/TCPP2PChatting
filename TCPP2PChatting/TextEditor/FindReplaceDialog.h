#ifndef _FINDREPLACEDIALOG_H
#define _FINDREPLACEDIALOG_H

#include "TextEditingForm.h"
#include "resource.h"

#include <afxdlgs.h>
#include <string>

using namespace std;

class FindReplaceDialog : public CFindReplaceDialog {
public:
	enum { IDF = IDCNT_FINDDIALOG, IDR = IDCNT_REPLACEDIALOG };
public:
	FindReplaceDialog(BOOL findOnly = TRUE, string findWhat = "", CWnd* parent = NULL);
	virtual BOOL OnInitDialog();

	BOOL WrapAround();

	BOOL Find();
	void Replace();

private:
	TextEditingForm* textEditingForm;
};

#endif //_FINDREPLACEDIALOG_H