$persistentObjectSet = new SimSet() {
      setType = "Persistent";

   new Sprite(emptySeatIcon) {
      image = "avatarEmptyImage";
      SceneLayer = "9";
      size = "0.500 0.500";
      Position = "15.222891 -4.447305";
      BodyType = "static";
         useMouseEvents = "1";
   };
   new Sprite(npc1) {
      image = "BlackJack_DougImage";
      internalName = "The Don";
      SceneLayer = "9";
      Position = "14.597492 -0.103852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=800000,aiStrategy=2,reactionTime=2,isAvailableOn1=0,isAvailableOn2=0";
   };
   new Sprite(npc2) {
      image = "BlackJack_BenImage";
      internalName = "Farmer Ben";
      SceneLayer = "9";
      Position = "15.847492 -0.103852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=250,aiStrategy=0";
   };
   new Sprite(npc3) {
      image = "BlackJack_MitchImage";
      internalName = "Hustler";
      SceneLayer = "9";
      Position = "17.097492 -0.103852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=100000.000,aiStrategy=3";
   };
   new Sprite(npc4) {
      image = "BlackJack_CassieImage";
      internalName = "Showgirl";
      SceneLayer = "9";
      Position = "18.347492 -0.103852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=200,aiStrategy=5,isAvailableOn1=0";
   };
   new Sprite(npc5) {
      image = "BlackJack_ElieImage";
      internalName = "Singer";
      SceneLayer = "9";
      Position = "14.597492 -1.353852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=500000,reactionTime=2,isAvailableOn1=0";
   };
   new Sprite(npc6) {
      image = "BlackJack_JDImage";
      internalName = "JD";
      SceneLayer = "9";
      Position = "15.847492 -1.353852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=300,isAvailableOn1=0";
   };
   new Sprite(npc7) {
      image = "BlackJack_JonImage";
      internalName = "Boxer";
      SceneLayer = "9";
      Position = "17.097492 -1.353852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=500,aiStrategy=2";
   };
   new Sprite(npc8) {
      image = "BlackJack_JDImage";
      internalName = "npc8";
      SceneLayer = "9";
      Position = "18.347492 -1.353852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=800000,isAvailable=0";
   };
   new Sprite(npc9) {
      image = "BlackJack_JDImage";
      internalName = "npc9";
      SceneLayer = "9";
      Position = "14.597492 -2.603852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=250,aiStrategy=2,reactionTime=2,isAvailable=0";
   };
   new Sprite(npc10) {
      image = "BlackJack_JDImage";
      internalName = "npc10";
      SceneLayer = "9";
      Position = "15.847492 -2.603852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=250,isAvailable=0";
   };
   new Sprite(npc11) {
      image = "BlackJack_JDImage";
      internalName = "npc11";
      SceneLayer = "9";
      Position = "17.097492 -2.603852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=250,aiStrategy=3,isAvailable=0";
   };
   new Sprite(npc12) {
      image = "BlackJack_JDImage";
      internalName = "npc12";
      SceneLayer = "9";
      Position = "18.347492 -2.603852";
      BodyType = "static";
         Behavior0 = "BlackjackAiPlayerBehavior=1,startingCash=250,isAvailable=0";
   };
   new Sprite(userAi) {
      image = "BlackJack_PlayerImage";
      SceneLayer = "9";
      Position = "13.534992 -2.635102";
      BodyType = "static";
         Behavior0 = "BlackjackUserPlayerBehavior=1";
   };
   new Sprite(currentHandIcon) {
      image = "TurnArrowSprite";
      SceneLayer = "2";
      size = "0.780 0.273";
      Position = "4.715961 -1.882039";
      BodyType = "static";
      Visible = "0";
   };
   new Sprite(shufflingIcon) {
      image = "ShufflingImage";
      size = "1.781 0.547";
      Position = "13.534437 2.411164";
      BodyType = "static";
   };
   new Sprite(bustIcon) {
      image = "BustImage";
      size = "0.891 0.273";
      Position = "15.038133 2.433117";
      BodyType = "static";
   };
   new Sprite(blackjackIcon) {
      image = "BlackjackImage";
      size = "1.118 0.337";
      Position = "17.250399 1.795953";
      BodyType = "static";
   };
   new Sprite(winIcon) {
      image = "WinImage";
      size = "0.891 0.273";
      Position = "17.381884 2.433117";
      BodyType = "static";
   };
   new Sprite(loseIcon) {
      image = "LoseImage";
      size = "0.891 0.273";
      Position = "18.553766 2.433117";
      BodyType = "static";
   };
   new Sprite(pushIcon) {
      image = "PushImage";
      size = "0.891 0.273";
      Position = "19.725641 2.433117";
      BodyType = "static";
   };
   new Sprite(BankruptIcon) {
      image = "BankruptSprite";
      size = "1.781 0.547";
      Position = "13.468750 1.343766";
      BodyType = "static";
   };
   new Sprite(BankBackgroundObject) {
      image = "GUI_BarSprite";
      SceneLayer = "2";
      size = "7.984 0.836";
      Position = "0.007414 -2.589969";
      BodyType = "static";
   };
   new BitmapFontObject(PlayerBankDisplay) {
      size = "0.000 0.250";
      Position = "-0.812500 -2.730000";
      BodyType = "static";
      image = "fontImage";
      textAlignment = "Right";
         frame = "0";
         Behavior0 = "UpdateFontFromTemplateBehavior=1,bitmapFontTemplate=bitmapFontTemplate";
   };
   new BitmapFontObject(bitmapFontTemplate) {
      size = "0.000 0.250";
      Position = "13.104938 0.598688";
      BodyType = "static";
      image = "Blackjack_FontsSprite";
      textAlignment = "Right";
         frame = "0";
   };
   new Sprite(CasinoCarpetBackground) {
      image = "Carpet_BackdropImage";
      SceneLayer = "30";
      size = "8.229 6.190";
      BodyType = "static";
   };
   new Sprite(BottomBarPlayerAvatarImage) {
      image = "BlackJack_PlayerImage";
      size = "0.764 0.750";
      Position = "-3.532266 -2.570313";
      BodyType = "static";
   };
   new Sprite(BottomBarAIBackgroundTemplate) {
      image = "GUI_Bar_AIcreditsSprite";
      SceneLayer = "1";
      size = "1.949 0.398";
      Position = "14.009750 -3.732758";
      BodyType = "static";
   };
};
