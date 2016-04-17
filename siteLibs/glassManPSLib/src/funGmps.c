void padding(char inpStr[], long len, long tot_len)
{
  if(len > tot_len) return;

  char tmp[tot_len];
  int i;
  for(i=0; i<tot_len; i++)
    tmp[i] = '0';

  int stpoint = tot_len - len;
  int idx = 0;
  for(i=stpoint; i<tot_len; i++)
  {
    tmp[i] = inpStr[idx];

    idx++;
  }
  for(i=0; i<tot_len; i++)
    inpStr[i] = tmp[i];
}

