#if defined _samp_keys_included
	#endinput
#endif
#define _samp_keys_included

const PRESSED_KEY_PACKET = 200;

stock OnKeyCallback(playerid, key_id) {
    printf("Игрок: %d. Нажал клавишу: %d", playerid, key_id);
    
    return 1;
}

ICustomRPC:PRESSED_KEY_PACKET(playerid, BitStream: bs) {
    new key_id = -1;
    BS_ReadValue(bs, PR_UINT32, key_id);

    return OnKeyCallback(playerid, key_id);
}