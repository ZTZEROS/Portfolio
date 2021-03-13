#pragma once

int ExecuteClassExperiment(int argumentCount, char* argumentVector[]);

class BaseClass
{
private:
	char variable1;

public:
	char variable2;
};

class DerivedClass : public BaseClass
{
private:
	char variable1;

public:
	char variable2;
	static char variable3;
};

static class StaticDerivedClass : public BaseClass
{
private:
	char variable1;

public:
	char variable2;
	static char variable3;
} STATIC_DERIVED_CLASS;

//warning C4091: 'static ' : ignored on left of 'Car' when no variable is declared