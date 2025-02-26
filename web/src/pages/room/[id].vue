<template>
    <Suspense>
        <Standby v-if="!isPlaying" :roomId="id" :users="users" @play="start" @append="append" />
        <Playing v-else :users="users" :answer="answer" @setAnswer="setAnswer" />
    </Suspense>
</template>

<script setup lang="ts">
import { ref } from 'vue';
import { useRoute } from 'vue-router';

const route = useRoute();
const id = route.params.id;

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