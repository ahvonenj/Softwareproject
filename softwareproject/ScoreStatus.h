#pragma once
//Scoren eri statukset, katsotaan mit� paketille on tapahtunut matkalla
enum ScoreStatus
	{
		DefaultStatus,
		Success,
		ServerError,
		InvalidName,
		ClientOutdated,
		InvalidScore
	};