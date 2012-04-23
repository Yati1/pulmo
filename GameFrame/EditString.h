#ifndef EDIT_STRING_H
#define EDIT_STRING_H

#include <string>

#include <OIS/OIS.h>

class EditString
{
public:
  EditString(void) : mInsert(true), mPosition(mText.begin()), mCaret(0)
  {}

  EditString( std::string newText )
  {
    setText( newText );
  }

  ~EditString(void)
  {}

protected:
  // The text for editing
  std::string mText;

  // Overwrite or insert
  bool mInsert;

  // Position for insert / overwrite
  std::string::iterator mPosition;

  // Caret Position - for positioning the cursor.
  int mCaret;

public:
  void setText( std::string & newText )
  {
    mText = newText;
    mPosition = mText.end();
    mCaret = (int)mText.length();
  }

  void addChar(char c)
	{
		if( mInsert || mPosition == mText.end() )
		{
			mPosition = mText.insert( mPosition, c);
		}
		else
		{
			*mPosition = c;
		}
		mPosition++;
		mCaret++;
	}

  std::string & getText() { return mText; }

  void clear()
  {
    mText.clear();
    mPosition = mText.end();
    mCaret = 0;
  }

  bool inserting() { return mInsert; }

  bool injectKeyPress( const OIS::KeyEvent );

  // gets the current position in the text for cursor placement
  int getPosition(void)const { return mCaret; }
};

#endif