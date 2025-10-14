#pragma once

class PaletteManager
{
private:
	struct PalletteMap
	{
		vector<COLORREF> base;
		vector<COLORREF> target;
	};


private:

	PalletteMap palette;


public:

	void imageSetting();

};

