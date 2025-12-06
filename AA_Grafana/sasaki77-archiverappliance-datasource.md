# Conversation
- conversationId: e462a332-44a7-40ee-a068-d84490d609c2
- endpoint: azureOpenAI
- title: New Chat
- exportAt: 15:12:15 GMT+0900 (한국 표준시)

## Options
- presetId: null
- model: gpt-5-chat
- modelLabel: ChatGPT 5 Chat
- promptPrefix: The assistant is GPT-5-Chat, an AI assistant based on the gpt-5-chat (preview) model, designed for advanced, natural, multimodal, and context-aware conversations for enterprise applications.

- temperature: 0.5
- resendFiles: true
- iconURL: https://chat.cborg.lbl.gov/assets/openai.png
- spec: openai/chatgpt-5
- maxContextTokens: 32768
- endpoint: azureOpenAI
- endpointType: undefined
- title: New Chat

## History
**User**
EPICS Archiver Appliance로 부터 grafana archiverappliance-datasource를 추가하는 방법을 자세히 알려줘.


**ChatGPT 5 Chat**
좋아요, 그러면 **EPICS Archiver Appliance**와 **Grafana**를 연동해서  
`grafana-archiverappliance-datasource` 플러그인을 추가하고 시각화하는 방법을  
단계별로 자세히 설명드릴게요.

---

## 1. 개념 정리
- **EPICS Archiver Appliance**: EPICS PV(Process Variable) 데이터를 장기간 저장하고 검색할 수 있는 아카이빙 시스템.
- **Grafana Archiver Appliance Datasource**: Grafana에서 Archiver Appliance REST API를 통해 PV 데이터를 읽어와 시각화할 수 있는 플러그인.

---

## 2. 사전 준비 사항
1. **EPICS Archiver Appliance 서버**가 동작 중이어야 함.
   - 웹 UI: `http://<archiver_host>:17665/mgmt`
   - REST API: `http://<archiver_host>:17668/retrieval`
2. **Grafana**가 설치되어 있어야 함.
   - 버전은 8.x 이상 권장.
3. Grafana 플러그인 설치를 위한 서버 접근 권한.

---

## 3. 플러그인 설치 방법

### 3.1 플러그인 다운로드
`grafana-archiverappliance-datasource`는 Grafana 공식 플러그인 저장소에 없을 수 있으므로, GitHub에서 직접 설치합니다.

```bash
cd /var/lib/grafana/plugins
git clone https://github.com/archiver-appliance/grafana-archiverappliance-datasource.git
```

### 3.2 Grafana 설정
Grafana가 외부 플러그인을 로드할 수 있도록 `grafana.ini`를 수정합니다.

```ini
[plugins]
allow_loading_unsigned_plugins = archiverappliance-datasource
```

> `grafana.ini`는 일반적으로 `/etc/grafana/grafana.ini`에 위치합니다.

### 3.3 Grafana 재시작
```bash
sudo systemctl restart grafana-server
```

---

## 4. Grafana에서 데이터소스 추가

1. **Grafana 웹 UI 접속**
   - `http://<grafana_host>:3000`
   - 관리자 계정으로 로그인.

2. **Configuration → Data sources → Add data source** 클릭.

3. 목록에서 **Archiver Appliance** 선택  
   (목록에 없다면 플러그인 설치가 잘 되었는지 확인).

4. **설정 입력**
   - **Name**: 예) `EPICS Archiver`
   - **URL**: Archiver Appliance Retrieval API 주소  
     예) `http://<archiver_host>:17668/retrieval`
   - **Access**: Server (default)

5. **Save & Test** 클릭 → 연결 성공 메시지 확인.

---

## 5. 대시보드에서 PV 데이터 시각화

1. **Create → Dashboard → Add new panel** 클릭.
2. **Query** 섹션에서 데이터소스를 `EPICS Archiver`로 선택.
3. PV 이름 입력 (예: `SR:CURRENT`).
4. 시간 범위를 지정하고, 시각화 타입(Line chart, Gauge 등) 선택.
5. 패널 저장.

---

## 6. 문제 해결 팁
- **404 오류**: Retrieval API URL이 잘못되었거나 방화벽 차단.
- **No data**: 해당 PV가 Archiver Appliance에 저장되지 않았거나 시간 범위가 잘못됨.
- **플러그인 목록에 없음**: Grafana 재시작 및 `allow_loading_unsigned_plugins` 설정 확인.

---

## 7. 참고 문서
- Archiver Appliance: [https://slacmshankar.github.io/epicsarchiver_docs/](https://slacmshankar.github.io/epicsarchiver_docs/)
- grafana-archiverappliance-datasource GitHub: [https://github.com/archiver-appliance/grafana-archiverappliance-datasource](https://github.com/archiver-appliance/grafana-archiverappliance-datasource)
- Grafana 플러그인 설정: [https://grafana.com/docs/grafana/latest/plugins/](https://grafana.com/docs/grafana/latest/plugins/)

---

💡 원하시면 제가 **docker-compose**로 Archiver Appliance + Grafana + 플러그인을 한 번에 띄우는 예제 구성 파일도 만들어 드릴 수 있습니다.  
그렇게 하면 설치와 설정이 훨씬 간단해집니다.

원하시나요?


