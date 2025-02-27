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
            <Standby v-if="!isPlaying" :roomId="id" :users="users" @play="start" @append="append('aaa')" />
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
  };

  ws.value.onmessage = (event: MessageEvent<string>) => {
    const data = JSON.parse(event.data);
    switch (data["type"]) {
      // TODO: 各レスポンス処理を実装する
      case "":
        
        break;
      case "":

        break;
      case "":
        // 次のラウンドに移行
        setAnswer(null);
        isLoading.value = false;
        title.value = ""; // TODO: レスポンスの値を入れるようにする
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
  await sleep(3000);
  sendMessage('{"type": "ready"}');
  isLoading.value = true;
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

const append = (username: string) => {
    users.value.push({
      username: username,
      isReady: false, 
      judging: false, 
      isSuccessed: null,
      score: 0
    });
}

const nextQuestion = (t: string) => {
  title.value = t;
  setAnswer(null);
}

const start = () => {
    isPlaying.value = true;
}

users.value = [
    {
      username: "thirdlf", 
      isReady: true, 
      judging: false, 
      score: 80,
      isSuccessed: null
    },
    {
      username: "naoido", 
      isReady: true, 
      judging: true, 
      score: 100,
      isSuccessed: null
    },
    {
      username: "ei", 
      isReady: true, 
      judging: false, 
      score: 60,
      isSuccessed: false
    },
    {
      username: "eii",
       isReady: true, 
       judging: false, 
       score: 70,
      isSuccessed: true
    },
];
</script>