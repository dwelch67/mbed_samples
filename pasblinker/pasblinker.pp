
var
    STCTRL   : DWord absolute $E000E010;
    STRELOAD : DWord absolute $E000E014;
    STCURR   : DWord absolute $E000E018;
    FIO1DIR2 : DWord absolute $2009C022;
    FIO1SET2 : DWord absolute $2009C03A;
    FIO1CLR2 : DWord absolute $2009C03E;

    SCS      : DWord absolute $400FC1A0;
    CLKSRCSEL: DWord absolute $400FC10C;
    PLL0FEED : DWord absolute $400FC08C;
    PLL0CON  : DWord absolute $400FC080;
    PLL0CFG  : DWord absolute $400FC084;
    PLL0STAT : DWord absolute $400FC088;
    CCLKCFG  : DWord absolute $400FC104;

procedure PLL_Init;
begin
    SCS       := $20;
    while ((SCS and $40) = 0) do
        ;
    CLKSRCSEL := 1;
    PLL0CFG   := 12-1;
    PLL0FEED  := $AA;
    PLL0FEED  := $55;
    PLL0CON   := 1;
    PLL0FEED  := $AA;
    PLL0FEED  := $55;
    while ((PLL0STAT and $01000000) = 0) do
        ;
    CCLKCFG   := 3-1;
    while ((PLL0STAT and $04000000) = 0) do
        ;
    PLL0CON   := 3;
    PLL0FEED  := $AA;
    PLL0FEED  := $55;
    while ((PLL0STAT and $02000000) = 0) do
        ;
end;


procedure STWait;
  begin
    while((STCTRL and $00010000) = 0) do
        ;
  end;




begin
    PLL_Init();

    FIO1DIR2 := FIO1DIR2 or $B4;

    STCTRL  :=$00000004;
    STRELOAD:=$00FFFFFF;
    STCTRL  :=$00000005;

    FIO1CLR2:=$B4;

  while true do
    begin
      FIO1SET2:=$80;
      FIO1CLR2:=$20;
      STWait();
      FIO1SET2:=$20;
      FIO1CLR2:=$80;
      STWait();
      FIO1SET2:=$10;
      FIO1CLR2:=$20;
      STWait();
      FIO1SET2:=$04;
      FIO1CLR2:=$10;
      STWait();
      FIO1SET2:=$10;
      FIO1CLR2:=$04;
      STWait();
      FIO1SET2:=$20;
      FIO1CLR2:=$10;
      STWait();
    end;
end.


