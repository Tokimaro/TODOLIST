#pragma once




class Bus
{
public:
	Bus();
	~Bus();
	Bus(int num, int len);
	int ShowNum();
	int ShowLen();
	int ShowPrice();
	void EditNum(int num);
	void EditLen(int len);
	virtual void Calculate(int len);
	void ChangeFullPrice(int fullpr);


private:
	int number;
	int length;
	int price = 45;
	int fullprice;
};

Bus::Bus()
{
	this->number = -1;
	this->length = -1;

}

Bus::Bus(int num, int len)
{
	this->number = num;
	this->length = len;
	Calculate(len);
}

Bus::~Bus()
{
}

int Bus::ShowNum() {
	if (number >= 0)
	{
		return number;
	}
	
}

void Bus::Calculate(int len) {
	fullprice = price * len;
}

int Bus::ShowLen() {
	if (length >= 0) {
		return length;
	}
	
}

void Bus::EditNum(int num) {
	if (number >= 0)
	{
		this->number = num;
	}

}

void Bus::EditLen(int len) {
	if (length >= 0)
	{
		this->length = len;
		Calculate(len);
	}

}

int Bus::ShowPrice() {
	return fullprice;
}

void Bus::ChangeFullPrice(int fullpr) {
	this->fullprice = fullpr;
}

class TrolleyBus : public Bus
{
public:
	TrolleyBus() :Bus() {
		this->electro = 0;
	}


	TrolleyBus(int num, int len, bool elec) :Bus(num, len) {
		this->electro = elec;
		Calculate(elec);
	}
	~TrolleyBus();
	void Calculate(bool elec) {
		int fullpr = Bus::ShowLen()*pricelec;
		Bus::ChangeFullPrice(fullpr);
	}


private:
	bool electro;
	int pricelec = 10;
};



bool operator == (Bus c1, int c2)
{
	return c1.ShowNum() == c2;
}

bool operator != (Bus c1, int c2)
{
	return c1.ShowNum() != c2;
}


TrolleyBus::~TrolleyBus() {

}
