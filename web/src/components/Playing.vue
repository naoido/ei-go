<template>
    <div class="w-100 h-100 d-flex justify-center align-center">
        <v-dialog v-model="dialog">
            <v-card class="d-flex justify-center align-center flex-column pa-5" v-if="dialog">
                <v-card-title style="font-style: italic;" class="text-h3">
                    回答
                </v-card-title>
                <div class="d-flex justify-center align-center">
                    <v-icon size="50">mdi-account</v-icon>
                    <v-card-text class="text-h4">{{ answer.username }}</v-card-text>
                </div>
                <v-card-subtitle class="text-h3" style="font-style: italic;">
                    {{ answer.text }}
                </v-card-subtitle>
                <v-card-title style="font-style: italic;" class="text-h3">
                    AI: 「{{ title }}」
                </v-card-title>
                <v-btn @click="del">消す</v-btn>
            </v-card>
        </v-dialog>
        <v-card class="d-flex justify-center align-center w-75 pa-5">
            <div class="d-flex flex-column justify-center align-center w-100">
                <v-card-title class="text-h3">
                    今回のお題({{ round }}/5)
                </v-card-title>
                <v-card-subtitle class="text-h2">
                    {{ title }}
                </v-card-subtitle>
                <v-btn @click="set">表示</v-btn>
            </div>
            <div class="d-flex flex-column justify-center align-center w-100 pl-16">
                <v-card-title class="text-h4">
                    プレイヤー
                </v-card-title>
                <v-sheet class="scrollable-list">
                    <transition-group name="list" tag="div">
                        <v-card
                            v-for="(item, index) in getSortedUser()"
                            :key="item.username"
                            class="d-flex align-center justify-space-between"
                            flat
                        >   
                            <div class="d-flex justify-center align-center">
                                <v-progress-circular
                                    :indeterminate="true"
                                    v-if="item.judging"
                                    size="20"
                                    color="blue-lighten-2"
                                    class="my-2"
                                />
                                <v-icon v-else
                                    :color="item.isSuccessed === null ? 'grey' : item.isSuccessed ? 'success' : 'error'"
                                >
                                    {{ item.isSuccessed == null ? "mdi-thought-bubble-outline" : item.isSuccessed ? "mdi-check-bold" : "mdi-close" }}
                                </v-icon>
                                <v-card-text style="font-size: 20px;">
                                    {{ item.username }}
                                </v-card-text>
                                <v-icon 
                                    v-if="index < 3"
                                    :color="index === 0 ? 'yellow-darken-2' : index === 1 ? 'blue-grey-lighten-1' : 'brown-darken-2'"
                                >
                                    mdi-crown
                                </v-icon>
                            </div>
                            <div>
                                score: {{ item.score }}
                            </div>
                        </v-card>
                    </transition-group>
                </v-sheet>
            </div>
        </v-card>
    </div>
</template>

<script setup lang="ts">
import { ref, watchEffect } from 'vue';

type User = {
    username: string,
    isReady: boolean,
    judging: boolean,
    score: number,
    isSuccessed: boolean | null
}

type Answer = {
    username: string,
    text: string
}

interface Props {
    roomId: string,
    users: User[],
    answer: Answer,
    title: string,
    round: number
}

const props = defineProps<Props>();
const emit = defineEmits(["setAnswer", "sortedUser"]);
const dialog = ref(false);

watchEffect(() => {
    dialog.value = props.answer !== null;
});

const del = () => {
    emit("setAnswer", null);
}
const set = () => {
    emit("setAnswer", {
        username: "naoido",
        text: "If you're making an iOS app, what language do you use?"
    });
}
const getSortedUser = () => [...props.users].sort((a, b) => b.score - a.score);
</script>

<style scoped>
.scrollable-list {
  height: 400px;
  width: 100%;
  overflow-y: auto;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 8px;
}
.list-enter-active, .list-leave-active {
  transition: all 0.5s ease;
}
.list-enter-from {
  opacity: 0;
  transform: translateX(-20px);
}
.list-leave-to {
  opacity: 0;
  transform: translateX(20px);
}
</style>