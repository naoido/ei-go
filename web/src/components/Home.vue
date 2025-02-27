<template>
    <div class="d-flex justify-center align-center h-100">
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
        <v-card class="w-75 d-flex jusify-center align-center flex-column pt-10 pb-16">
            <v-card-title class="text-h2 text-center mb-10" style="font-style: italic;">
                英GO!!
            </v-card-title>
            <v-btn style="font-size: 30px;" height="60px" class="w-50 my-2" color="success" @click="onRoomClick">
                ルーム作成
            </v-btn>
            <div class="d-flex justify-center align-center mt-5">
                <v-btn style="font-size: 30px;" height="60px" class="w-100 mx-2" color="indigo-accent-3">
                    単語追加
                </v-btn>
                <v-btn style="font-size: 30px;" height="60px" class="w-100 mx-2" color="teal-lighten-1">
                    一覧
                </v-btn>
            </div>
        </v-card>
    </div>
</template>

<script setup lang="ts">
import axios from 'axios';
import { ref } from 'vue';
import { useRouter } from 'vue-router';

const router = useRouter();

const isLoading = ref(false);

const getRoomData = async (): Promise<any> => {
    const data = (await axios.post(`https://hackz.naoido.com/api/room/create`)).data;
    return data;
}

const onRoomClick = async () => {
    isLoading.value = true;
    const data = await getRoomData();

    localStorage.setItem("admin_token", data.admin_token);
    router.push({ path: `/room/${data.roomId}`});

    isLoading.value = false;
}
</script>   