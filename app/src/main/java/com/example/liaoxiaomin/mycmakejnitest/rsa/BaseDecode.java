package com.example.liaoxiaomin.mycmakejnitest.rsa;

/**
 * Created by zuozhuang on 2017/12/27.
 */
public class BaseDecode {
    static char base64char[]  = "LMNOPQRSTfghijklmnopqrsABCDEFGHIJKtuvwxyz012UVWXYZabcde3456789+/".toCharArray();

    public static String DeCode(String code){
        char base64[] = code.toCharArray();
        char bindata[] = new char[100];
        String decode = null;
        int i, j;
        char k;
        char temp[]= new char[4];
        for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
        {
            for ( k = 0 ; k < 64 ; k ++ )
            {
                if ( base64char[k] == base64[i] )
                    temp[0]= k;
            }
            for ( k = 0 ; k < 64 ; k ++ )
            {
                if ( base64char[k] == base64[i+1] )
                    temp[1]= k;
            }
            for ( k = 0 ; k < 64 ; k ++ )
            {
                if ( base64char[k] == base64[i+2] )
                    temp[2]= k;
            }
            for ( k = 0 ; k < 64 ; k ++ )
            {
                if ( base64char[k] == base64[i+3] )
                    temp[3]= k;
            }

            bindata[j++] = (char) (((char)(((char)(temp[0] << 2))&0xFC)) | (( char)((char)(temp[1]>>4)&0x03)));
            if ( base64[i+2] == '=' )
                break;

            bindata[j++] = (char) (((char)(((char)(temp[1] << 4))&0xF0)) |
                        ((char)((char)(temp[2]>>2)&0x0F)));
            if ( base64[i+3] == '=' )
                break;

            bindata[j++] = (char) (((char)(((char)(temp[2] << 6))&0xF0)) |
                        ((char)(temp[3]&0x3F)));
        }
        decode = new String(bindata);
        return decode.trim();
    }
}
