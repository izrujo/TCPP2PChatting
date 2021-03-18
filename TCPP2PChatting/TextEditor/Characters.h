//Characters.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Manage text data.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2019.05
**************************************************/

#ifndef _CHARACTERS_H
#define _CHARACTERS_H

#include "Glyph.h"

class Character : public Glyph {
public:
	Character(int color = 0x000000);
	Character(const Character& source);
	virtual ~Character() = 0;
	Character& operator=(const Character& source);

	virtual void Paint(int color);
	virtual void Select(bool isSelected);

	virtual int GetColor() const;
	virtual bool GetIsSelected() const;
	virtual void Accept(Visitor* visitor);
protected:
	int color;
	bool isSelected;
};

inline int Character::GetColor() const {
	return this->color;
}

inline bool Character::GetIsSelected() const {
	return this->isSelected;
}

class SingleByteCharacter : public Character {
public:
	SingleByteCharacter(int color = 0x000000);
	SingleByteCharacter(char content, int color = 0x000000);
	SingleByteCharacter(const SingleByteCharacter& source);
	virtual ~SingleByteCharacter();
	SingleByteCharacter& operator=(const SingleByteCharacter& source);

	bool IsEqual(const SingleByteCharacter& other);
	bool IsNotEqual(const SingleByteCharacter& other);

	bool operator==(const SingleByteCharacter& other);
	bool operator !=(const SingleByteCharacter& other);

	virtual Glyph* Clone();
	virtual string GetContent();
	char GetChar() const;
private:
	char content;
};

inline char SingleByteCharacter::GetChar() const {
	return this->content;
}

class DoubleByteCharacter : public Character {
public:
	DoubleByteCharacter(int color = 0x000000);
	DoubleByteCharacter(const char(*content), int color = 0x000000);
	DoubleByteCharacter(const DoubleByteCharacter& source);
	virtual ~DoubleByteCharacter();
	DoubleByteCharacter& operator=(const DoubleByteCharacter& source);

	bool IsEqual(const DoubleByteCharacter& other);
	bool IsNotEqual(const DoubleByteCharacter& other);

	bool operator==(const DoubleByteCharacter& other);
	bool operator !=(const DoubleByteCharacter& other);

	virtual Glyph* Clone();
	virtual string GetContent();
private:
	char content[2];
};

#endif //_CHARACTERS_H