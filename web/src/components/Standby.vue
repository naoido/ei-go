<template>
    <div class="w-100 h-100 d-flex justify-center align-center flex-column">
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
        <v-card class="w-75 d-flex justify-center align-center flex-row py-10">
            <div class="w-50 d-flex justify-center flex-column align-center">
                <div class="mb-5">
                    <v-card-title>ルームID</v-card-title>
                    <v-card-subtitle>
                        {{ roomId }}
                    </v-card-subtitle>
                </div>
                <vue-qrcode :value="roomId" :options="{ width: 200, margin: 1 }" class="mb-4" />
                <v-btn :color="isReady() ? 'success' : 'blue-grey-lighten-'" :disabled="!isReady()" @click="play">スタート</v-btn>
            </div>
            <div class="w-100 pr-5">
                <h1>参加ユーザー({{ users.filter(u => u.isReady).length }}/{{ users.length }})</h1>
                <v-sheet class="scrollable-list">
                    <transition-group name="list" tag="div">
                        <v-card
                            v-for="item in users"
                            :key="item.id"
                            class="d-flex align-center"
                            flat
                        >
                            <v-icon :color="item.isReady ? 'success' : 'error'" @click="item.isReady = !item.isReady">
                                {{ item.isReady ? "mdi-check-bold" : "mdi-close" }}
                            </v-icon>
                            <v-card-text style="font-size: 20px;">
                                {{ item.username }}
                            </v-card-text>
                        </v-card>
                    </transition-group>
                </v-sheet>
            </div>
        </v-card>
    </div>
</template>

<script setup lang="ts">
import VueQrcode from "@chenfengyuan/vue-qrcode";
import { ref } from "vue";

type User = {
    id: string,
    username: string,
    isReady: boolean,
}

interface Props {
    roomId: string,
    users: User[]
}

const emit = defineEmits(["play"]);
const props = defineProps<Props>();

const roomId = props.roomId;
const users = props.users;
const isLoading = ref(false);

const isReady = () => {
    return users.length > 1 && users.find(u => !u.isReady) == null
}

const play = () => {
    emit("play");
}
</script>

<style scoped>
.scrollable-list {
  height: 400px;
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