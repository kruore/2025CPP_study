#pragma once

class employee {

	enum class Title
	{
		Admin,
		Enginner
	};
public:
	employee() : grade(0), title(Title::Enginner) {};

	void Hire() {};
	void Fire() {};

	void UpGrade() { grade++; };
	void DownGrade() { grade--; };

	int GetGrade() { return grade; };

private:

	int grade;
	Title title;


};