<template>
    <div class="w-100 h-100">
        <div 
            v-if="isLoading" 
            class="position-fixed d-flex justify-center align-center w-100 h-100" 
            style="background-color: #000000aa; z-index: 100;"
        >
            <v-progress-circular
                :indeterminate="isLoading"
                size="100"
                color="primary"
                class="my-2"
            />
        </div>
        <Suspense>
            <Standby v-if="!isPlaying" :roomId="id" :users="users" @play="start" />
            <Ranking v-else-if="isRanking" :users="users"/>
            <Playing 
              v-else 
              :users="users" 
              :answer="answer" 
              :title="title" 
              :round="round"
              @setAnswer="setAnswer"
            />
        </Suspense>
    </div>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue';
import { useRoute } from 'vue-router';

type User = {
  id: string,
  username: string,
  isReady: boolean,
  judging: boolean,
  isSuccessed: boolean | null,
  score: number,
}

type Answer = {
  username: string,
  text: string
}

const hogeAnswer = {
    username: "naoido",
    text: "If you're making an iOS app, what language do you use?"
}

const users = ref<User[]>([]);
const answer = ref<Answer | null>(hogeAnswer);

const route = useRoute();
const id = route.params.id;

const isPlaying = ref(false);
const isLoading = ref(true);
const isRanking = ref(false);
const self = ref("");

const title = ref("Swift");
const round = ref(1);

//** WEBSOCKET */
const wsUrl = `wss://hackz.naoido.com/ws?roomId=${id}`;
const ws = ref<WebSocket | null>(null);

const connectWebSocket = () => {
  ws.value = new WebSocket(wsUrl);

  ws.value.onopen = () => {
    console.log("WebSocket 接続成功");
    isLoading.value = false;
    sendMessage('{"type": "update","name": "brawser","isReady": true}')
  };

  ws.value.onmessage = (event: MessageEvent<string>) => {
    const data = JSON.parse(event.data);
    switch (data["type"]) {
      case "new_player":
        append(data["playerId"], data["name"]);
        break;
      case "update":
        const user = users.value.find(u => u.id === data["playerId"]);
        if (user) user.username = data["name"];
        if(data["isReady"]) {
          users.value.find(u => u.id === data["playerId"])!.isReady = true;
        }
        break;
      case "gameJoined":
        self.value = data["playerId"];
      case "result":
        next();
      case "question":
        setAnswer(null);
        isLoading.value = false;
        title.value = data["question"];
        break;
      default:
        break;
    }
  };

  ws.value.onclose = () => {
    console.log("WebSocket 接続が閉じられました");
  };

  ws.value.onerror = (error) => {
    console.error("WebSocket エラー:", error);
  };
};

const sendMessage = (value: string) => {
  if (ws.value && ws.value.readyState === WebSocket.OPEN) {
    ws.value.send(value);
  } else {
    console.warn("WebSocket が接続されていません");
  }
};

const next = async () => {
  await sleep(2000);
  isLoading.value = true;
  start();
}

onMounted(() => {
  connectWebSocket();
});

onUnmounted(() => {
  if (ws.value) {
    ws.value.close();
  }
});
//** --- */

const setAnswer = (value: Answer | null) => answer.value = value;
const sleep = (time: number) => new Promise((resolve) => setTimeout(resolve, time));
const getSortedUser = () => [...users.value].sort((a, b) => b.score - a.score);

const append = (id: string, username: string) => {
    users.value.push({
      id: id,
      username: username,
      isReady: false, 
      judging: false, 
      isSuccessed: null,
      score: 0
    });
}

const start = () => {
    isPlaying.value = true;
    sendMessage(`{"type": "gameStart","admin_token": "${localStorage.getItem("admin_token")}"}`);
}
</script>