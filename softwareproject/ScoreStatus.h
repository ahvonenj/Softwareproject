#pragma once
//Scoren eri statukset, katsotaan mitä paketille on tapahtunut matkalla
enum ScoreStatus
	{
		DefaultStatus,
		Success,
		ServerError,
		InvalidName,
		ClientOutdated,
		InvalidScore
	};