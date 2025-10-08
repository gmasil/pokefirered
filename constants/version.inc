	.ifdef FIRERED
	.equiv GAME_VERSION, 4
	.else
	.ifdef GYARADOS
	.equiv GAME_VERSION, 4
	.else
	.equiv GAME_VERSION, 5
	.endif
	.endif

	.ifdef ENGLISH
	.equiv GAME_LANGUAGE, 2
	.else
	.ifdef GERMAN
	.equiv GAME_LANGUAGE, 5
	.endif
	.endif
