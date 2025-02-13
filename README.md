# Sparta_08

1. GameState와 GameInstance를 활용한 게임 로직 구현
  - GameInstance
    > SceneHandle, UIHandle, SoundHandle class를 구현하여 GameInstance에서 게임 전반에 대한 로직 구현.
    > 게임 종료 기능은 반드시 GameInstance를 통하여 처리하도록 구현. (게임이 종료 될 때 필요한 데이터들을 저장해야할 수 있기 때문)
    > Level 이동은 GameInstnace에 요청하면 GameInstance가 SceneHandle을 통해 기능 수행. (Level 전환 시 필요한 기능 처리를 위함)
    > GameState가 초기화 되면 UIHandle의 UserWidget들의 옵저버를 GameState에 Add 하는 기능 구현 (현재 Level의 게임 데이터나 플레이어의 정보가 변동 되었을 때 UI에 즉각 반영하기 위함)
    
      - SceneHandle
          > 이동할 Scene의 이름을 매개변수로 전달받아 Level의 이동을 구현 (매개변수는 enum 값으로 전달 받고, enum 값에 따라 이동할 Level 선택)
          > 현재의 Level이 어디인지 저장하는 변수를 관리, 전달
          > 현재의 Level 값을 제대로 가지고 있는지 확인하는 체크 함수 구현
          
      - UIHandle
          > 초기화 시 필요한 UserWidget을 Create하고 변수에 저장하였다가, 함수의 호출에 따라 Viewport에 출력하는 기능 수행
          > 각각의 UserWidget의 필요 기능은 UserWidget 클래스를 상속 받은 BaseWidget class의 파생 클래스들이 처리
          > Level 이동에 대한 버튼 입력 처리는 GameInstance에 요청. GameInstance가 SceneHandle에 접근하여 Level 이동 처리
          
      - SoundHandle
          > 초기화 시, 게임에 사용될 SoundBase 목록 Load, Audio Component들 생성
          > 새 Level에 진입할 때, GameInstance를 통해 현재 Level을 확인하고 그에 적합한 BGM이 출력되도록 구현
          > UserWidget에 마우스 클릭 이벤트가 발생하면 해당 사운드를 출력하도록 구현
          > UI를 통해 사운드 볼륨을 조절하면 그에 맞는 Audio Component들의 Volume이 적용되도록 구현
          
  - GameState
    > Player가 동작 준비가 완료되는 시점 (Begin Play)을 전달받아 현재 레벨의 로직을 수행(현재 레벨이 무엇인지는 GameInstance에게 전달받음)
    > GameState가 준비 완료되면 GameInstance에 이를 알려 동작을 수행하도록 함 (Level, GameState, PlayerController, Viewport 등이 준비 되어있어야 하는 경우를 위해)
    > 시간 경과에 따라 Wave가 변동되도록 구현, Wave 변화 시마다 아이템을 Spawn 하도록 구현
    > Wave에 따라 필요한 이벤트를 발생 시키도록 구현 (2 : 장애물 맵 생성, 3 : 랜덤 운석 낙하)
    > Level과 Wave 수치에 따라 아이템 스폰 개수와 종류에 따른 확률이 변동되도록 구현
    > 위의 기능은 각각의 Spawner(Item Spawner, Meteor Spawner, Obstacles Spawner)에 값을 전달하여 그에 맞는 기능을 수행하도록 함
    > Wave가 모두 진행되면 다음 Level로 이동하는 Portal이 생성되도록 구현
    > Portal에 플레이어가 Overlap되면 입력 키를 알려주는 3D 위젯 활성화, Overlap 되지 않으면 비활성화 되도록 구현
    > Portal에 Overlap 된 상태에서 N 키를 입력하면 GameInstance에 다음 레벨로 이동해야함을 알림(GameInstance의 Level 이동 함수 호출)
    > 플레이어의 Hp가 0이 되면 GameOver 이벤트 기능을 수행
    
2. UserWidget을 활용한 UI 구현
  - GameStateObserver : GameState를 관찰하여 필요한 정보를 동기화하기 위한 인터페이스 (플레이어의 체력, 상태 등과 현재의 Level, Wave, PlayTime, Score 변동시 반영) 
  - BaseWidget : UserWidget 클래스를 상속받은 Widget 클래스. 구현할 Widget들의 공통 기능을 구현 및 Observer 인터페이스 상속
  - FadeWidget : Level 이동, 진입 시에 Fade In, Fade Out 애니메이션을 출력할 Widget 클래스
  - OptionWidget : 타이틀이나 인게임에서 옵션 호출 시 출력될 Widget 클래스. 게임 사운드의 볼륨을 조작할 수 있다. (인게임에서는 O 키를 입력 받으면 활성화)
  - TitleWidget : 타이틀 레벨에 출력되는 Widget 클래스. 게임 시작, 옵션, 게임 종료 버튼을 가지고 있다.
  - InGameWidget : 3개 난이도의 레벨에서 출력되는 Widget 클래스. 체력, 스코어, 상태이상, 스테이지, 웨이브, 플레이 시간 등을 출력하는 기능 수행
  - EndingWidget : 모든 난이도를 클리어하면 진입하는 엔딩 레벨에 출력되는 Widget 클래스. 개발 정보와 총 플레이 시간, 총 점수가 출력되는 엔딩 크래딧 애니메이션 출력 및 타이틀로 이동, 게임 종료 버튼
