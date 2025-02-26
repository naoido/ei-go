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
            <Standby v-if="!isPlaying" :roomId="id" :users="users" @play="start" @append="append" />
            <Playing v-else :users="users" :answer="answer" @setAnswer="setAnswer" />
        </Suspense>
    </div>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted, ref } from 'vue';
import { useRoute } from 'vue-router';

const route = useRoute();
const isLoading = ref(true);
const id = route.params.id;


const wsUrl = "ws://localhost:8080/ws";
const ws = ref<WebSocket | null>(null);
const messages = ref<string[]>([]);

const connectWebSocket = () => {
  ws.value = new WebSocket(wsUrl);

  ws.value.onopen = () => {
    console.log("WebSocket 接続成功");
    sendMessage("hello");
    isLoading.value = false;
  };

  ws.value.onmessage = (event: MessageEvent<string>) => {
    messages.value.push(event.data);
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

onMounted(() => {
  connectWebSocket();
});

onUnmounted(() => {
  if (ws.value) {
    ws.value.close();
  }
});


type User = {
    username: string,
    isReady: boolean,
}

type Answer = {
    username: string,
    text: string
}

const hogeAnswer = {
    username: "naoido",
    text: "If you're making an iOS app, what language do you use?"
}

const answer = ref<Answer | null>(hogeAnswer);
const setAnswer = (value: Answer | null) => answer.value = value;

const users = ref<User[]>([
    {username: "thirdlf", isReady: true},
    {username: "naoido", isReady: true},
    {username: "ei", isReady: true},
]);

const append = () => {
    users.value.push({username: "ei", isReady: true})
}

const isPlaying = ref(false);
const start = () => {
    isPlaying.value = true;
}
</script>